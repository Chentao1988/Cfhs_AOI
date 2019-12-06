#include "cfhs_savepoint.h"


Cfhs_SavePoint::Cfhs_SavePoint(QWidget *parent)
    : QWidget(parent)
{
    resize(900, 600);
    Init();
    setWindowStyle();
}

Cfhs_SavePoint::~Cfhs_SavePoint()
{
}

void Cfhs_SavePoint::LoadPath(QString path, QList<NPoint> &npoint)
{
    QByteArray temp_path = path.toLocal8Bit();
    std::string str_path;
    str_path.append(temp_path);
    cv::Mat Img_Mat1 = cv::imread(str_path, -1);
    Img_Qimg = Mat2QImage(Img_Mat1);
    save_map = QPixmap::fromImage(Img_Qimg);
    int width = qMin(save_map.width(), this->width());
    int height = static_cast<int>(width * 1.0 / (save_map.width() * 1.0 / save_map.height()));
    height = qMin(height, this->height());
    width = static_cast<int>(height * 1.0 * (save_map.width() * 1.0 / save_map.height()));
    save_width = width;
    save_height = height;
    sub_height = (float)save_map.height() / (float)save_height;
    sub_width = (float)save_map.width() / (float)save_width;
    nPoint = list_imgtothis(npoint);
    this->update();
}

QList<NPoint> Cfhs_SavePoint::GetList()
{
    return list_thistoimg(nPoint);
}

void Cfhs_SavePoint::onZoomInImage(void)
{
    m_ZoomValue *= 1.1;
    this->update();
}

void Cfhs_SavePoint::onZoomOutImage(void)
{
    m_ZoomValue /= 1.1;
    if (m_ZoomValue < 0.2)
        m_ZoomValue = 0.2;

    this->update();
}

void Cfhs_SavePoint::onPresetImage(void)
{
    m_ZoomValue = 1.0;
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    this->update();
}

void Cfhs_SavePoint::Init()
{
    pLayout = new QHBoxLayout();
    pButtonGroup = new QButtonGroup(this);
    button_zoomin = getButton("zoom_in");
    button_zoomout = getButton("zoom_out");
    button_adaptive = getButton("adaptive");
    button_rect = getButton("rectangle");
    button_elli = getButton("circle");
//	button_line = getButton("polygon");
    button_move = getButton("move");
    button_del = getButton("delete");
    connect(button_zoomin, SIGNAL(clicked()), this, SLOT(onZoomInImage()));
    connect(button_zoomout, SIGNAL(clicked()), this, SLOT(onZoomOutImage()));
    connect(button_adaptive, SIGNAL(clicked()), this, SLOT(onPresetImage()));
    connect(button_rect, SIGNAL(clicked()), this, SLOT(ClickButton_rect()));
    connect(button_elli, SIGNAL(clicked()), this, SLOT(ClickButton_elli()));
//	connect(button_line, SIGNAL(clicked()), this, SLOT(ClickButton_line()));
    connect(button_move, SIGNAL(clicked()), this, SLOT(ClickButton_move()));
    connect(button_del, SIGNAL(clicked()), this, SLOT(ClickButton_del()));
    pLayout->addStretch();
    pLayout->setSpacing(10);
    this->setLayout(pLayout);

    m_ZoomValue = 1.0;
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    m_OldPos = QPoint(0, 0);
    m_Pressed = false;
    select = 0;
}

void Cfhs_SavePoint::setWindowStyle()
{
    this->setStyleSheet("Cfhs_SavePoint{background:#000A19}");
}

QImage Cfhs_SavePoint::Mat2QImage(cv::Mat& mat)
{
    switch (mat.type())
    {
        // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
        return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable;
        // only create our color table once
        if (sColorTable.isEmpty())
        {
            for (int i = 0; i < 256; ++i)
                sColorTable.push_back(qRgb(i, i, i));
        }
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        image.setColorTable(sColorTable);
        return image;
    }
    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        break;
    }
    return QImage();
}

cv::Mat Cfhs_SavePoint::QImage2Mat(const QImage & image, bool inCloneImageData)
{
    switch (image.format())
    {
        // 8-bit, 4 channel
    case QImage::Format_RGB32:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return (inCloneImageData ? mat.clone() : mat);
    }

    // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        if (!inCloneImageData) {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
        }
        QImage swapped = image.rgbSwapped();
        return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
    }

    // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());

        return (inCloneImageData ? mat.clone() : mat);
    }

    default:
        qDebug("Image format is not supported: depth=%d and %d format\n", image.depth(), image.format());
        break;
    }
    return cv::Mat();
}

void Cfhs_SavePoint::CdrawEllipse()
{
    QPainter painter(this);
    painter.setPen(Qt::red);//设置画笔颜色
    QRect temp(p_Start, p_End);
    painter.drawEllipse(temp);//画圆
}


void Cfhs_SavePoint::SaveEllipse()
{
    NPoint point_temp;
    QPoint st_point;
    QPoint se_point;
    st_point.setX(p_Start.x() - m_XPtInterval);
    st_point.setY(p_Start.y() - m_YPtInterval);
    se_point.setX(p_End.x() - m_XPtInterval);
    se_point.setY(p_End.y() - m_YPtInterval);
    st_point.setX(int(this->width() / 2 - (this->width() / 2 - st_point.x()) / m_ZoomValue));
    st_point.setY(int(this->height() / 2 - (this->height() / 2 - st_point.y()) / m_ZoomValue));
    se_point.setX(int(this->width() / 2 - (this->width() / 2 - se_point.x()) / m_ZoomValue));
    se_point.setY(int(this->height() / 2 - (this->height() / 2 - se_point.y()) / m_ZoomValue));
    point_temp.Shape = 0;
    point_temp.Start_Point = st_point;
    point_temp.End_Point = se_point;
    nPoint << point_temp;
}

void Cfhs_SavePoint::CRect()
{
    QPainter painter(this);
    painter.setPen(Qt::red);//设置画笔颜色
    QRect temp(p_Start, p_End);
    painter.drawRect(temp);
}

void Cfhs_SavePoint::Show_map(int x, int y)
{
    for (int i = 0;i < nPoint.size();i++) {
        if (nPoint[i].Shape == 1) {
            int st_x, st_y;
            st_x = nPoint[i].Start_Point.x() + m_XPtInterval;
            st_y = nPoint[i].Start_Point.y() + m_YPtInterval;
            QRect temp(st_x, st_y, nPoint[i].End_Point.x() - nPoint[i].Start_Point.x(),
                nPoint[i].End_Point.y() - nPoint[i].Start_Point.y());
            QPainter painter(this);
            painter.setPen(Qt::red);//设置画笔颜色
            painter.translate(x, y);
            painter.scale(m_ZoomValue, m_ZoomValue);
            painter.translate(-x, -y);
            painter.drawRect(temp);//画矩形
        }
        else if (nPoint[i].Shape == 0) {
            int st_x, st_y;
            st_x = nPoint[i].Start_Point.x() + m_XPtInterval;
            st_y = nPoint[i].Start_Point.y() + m_YPtInterval;
            QRect temp(st_x, st_y, nPoint[i].End_Point.x() - nPoint[i].Start_Point.x(),
                nPoint[i].End_Point.y() - nPoint[i].Start_Point.y());
            QPainter painter(this);
            painter.setPen(Qt::red);//设置画笔颜色
            painter.translate(x, y);
            painter.scale(m_ZoomValue, m_ZoomValue);
            painter.translate(-x, -y);
            painter.drawEllipse(temp);//画圆形
        }
    }
}

void Cfhs_SavePoint::SaveRect()
{
    NPoint point_temp;
    QPoint st_point;
    QPoint se_point;
    st_point.setX(p_Start.x() - m_XPtInterval);
    st_point.setY(p_Start.y() - m_YPtInterval);
    se_point.setX(p_End.x() - m_XPtInterval);
    se_point.setY(p_End.y() - m_YPtInterval);
    st_point.setX(int(this->width() / 2 - (this->width() / 2 - st_point.x()) / m_ZoomValue));
    st_point.setY(int(this->height() / 2 - (this->height() / 2 - st_point.y()) / m_ZoomValue));
    se_point.setX(int(this->width() / 2 - (this->width() / 2 - se_point.x()) / m_ZoomValue));
    se_point.setY(int(this->height() / 2 - (this->height() / 2 - se_point.y()) / m_ZoomValue));
    point_temp.Shape = 1;
    point_temp.Start_Point = st_point;
    point_temp.End_Point = se_point;
    nPoint << point_temp;
}

void Cfhs_SavePoint::delectpoint(int x, int y)
{
    int point_x = x - m_XPtInterval;
    point_x = int(this->width() / 2 - (this->width() / 2 - point_x) / m_ZoomValue);
    int point_y = y - m_YPtInterval;
    point_y = int(this->height() / 2 - (this->height() / 2 - point_y) / m_ZoomValue);
    for (int i = 0;i < nPoint.size();i++) {
        if ((point_x > nPoint[i].Start_Point.x() && point_y > nPoint[i].Start_Point.y() &&
            point_x < (nPoint[i].End_Point.x()) &&
            point_y < (nPoint[i].End_Point.y()))
            || (point_x < nPoint[i].Start_Point.x() && point_y < nPoint[i].Start_Point.y() &&
            point_x > (nPoint[i].End_Point.x()) &&
            point_y > (nPoint[i].End_Point.y()))
            ) {
            nPoint.removeAt(i);
        }
    }
}

QString Cfhs_SavePoint::getFunctionButtonStyle(const QString &name)
{
    QString style = QString("QPushButton{image:url(:/%1_normal.png);"
        "background:transparent; border:none;}"
        "QPushButton:pressed{image:url(:/%2_press.png)}"
        "QPushButton:checked{image:url(:/%3_press.png);"
        "border: 1px solid red}").arg(name).arg(name).arg(name);

    return style;
}

QPushButton * Cfhs_SavePoint::getButton(const QString & name)
{
    QPushButton *button = new QPushButton(this);
    button->setFixedSize(35, 35);
    QString style = getFunctionButtonStyle(name);
    button->setStyleSheet(style);
    button->setCheckable(true);
    pLayout->addWidget(button, 0, Qt::AlignLeft | Qt::AlignTop);
    pButtonGroup->addButton(button);

    return button;
}

QPointF Cfhs_SavePoint::list_thistoimg(QPointF listpoint)
{
    QPointF point_temp;
    float x_len = (this->width() - save_width) / 2;
    float y_len = (this->height() - save_height) / 2;
    float x = (listpoint.x() - x_len)*save_map.width() / save_width;
    float y = (listpoint.y() - y_len)*save_map.height() / save_height;
    point_temp.setX(x);
    point_temp.setY(y);
    return point_temp;
}

QPointF Cfhs_SavePoint::list_imgtothis(QPointF listpoint)
{
    QPointF point_temp;
    float x_len = (this->width() - save_width) / 2;
    float y_len = (this->height() - save_height) / 2;
    float x = (listpoint.x())*save_width / save_map.width() + x_len;
    float y = (listpoint.y())*save_height / save_map.height() + y_len;
    point_temp.setX(x);
    point_temp.setY(y);
    return point_temp;
}

QList<NPoint> Cfhs_SavePoint::list_thistoimg(QList<NPoint> listpoint)
{
    QList<NPoint>return_qlist;
    NPoint point_temp;
    for (size_t i = 0; i < listpoint.size(); i++)
    {
        point_temp.Shape = listpoint[i].Shape;
        point_temp.Start_Point = list_thistoimg(listpoint[i].Start_Point);
        point_temp.End_Point = list_thistoimg(listpoint[i].End_Point);
        return_qlist << point_temp;
    }
    return return_qlist;
}

QList<NPoint> Cfhs_SavePoint::list_imgtothis(QList<NPoint> listpoint)
{
    QList<NPoint>return_qlist;
    NPoint point_temp;
    for (size_t i = 0; i < listpoint.size(); i++)
    {
        point_temp.Shape = listpoint[i].Shape;
        point_temp.Start_Point = list_imgtothis(listpoint[i].Start_Point);
        point_temp.End_Point = list_imgtothis(listpoint[i].End_Point);
        return_qlist << point_temp;
    }
    return return_qlist;
}




void Cfhs_SavePoint::paintEvent(QPaintEvent *event)
{
    // 绘制样式
    QPainter painter(this);
    if (save_map.isNull())	 return QWidget::paintEvent(event);
    // 根据窗口计算应该显示的图片的大小
    int width = qMin(save_map.width(), this->width());
    int height = static_cast<int>(width * 1.0 / (save_map.width() * 1.0 / save_map.height()));
    height = qMin(height, this->height());
    width = static_cast<int>(height * 1.0 * (save_map.width() * 1.0 / save_map.height()));
    /*记录 长宽缩小比率，获取鼠标坐标使用 */

    int xPos = this->width() / 2 + m_XPtInterval;
    int yPos = this->height() / 2 + m_YPtInterval;
    // 平移
    painter.translate(xPos, yPos);
    // 缩放
    painter.scale(m_ZoomValue, m_ZoomValue);
    // 绘制图像
    QRect picRect(-width / 2, -height / 2, width, height);
    painter.drawPixmap(picRect, save_map);
    if (m_Pressed != false) {
        switch (select) {
        case 0:
            break;
        case 1:
            CRect();
            break;
        case 2:
            CdrawEllipse();
            break;
        case 3:

            break;
        default:
            break;
        }
    }
    Show_map(xPos, yPos);
}

void Cfhs_SavePoint::wheelEvent(QWheelEvent * event)
{
    int value = event->delta();
    if (value > 0) {
        onZoomInImage();

    }
    else {
        onZoomOutImage();
    }
    this->update();
}

void Cfhs_SavePoint::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        p_Start = event->pos();
        p_End = event->pos();
        m_OldPos = event->pos();
        m_Pressed = true;
        if (select == 4)
        {
            delectpoint(m_OldPos.x(), m_OldPos.y());
        }
    }
    this->update();
}

void Cfhs_SavePoint::mouseMoveEvent(QMouseEvent * event)
{
    if (select == 0) {
        if (m_Pressed) {
            QPoint pos = event->pos();
            int xPtInterval = pos.x() - m_OldPos.x();
            int yPtInterval = pos.y() - m_OldPos.y();
            m_XPtInterval += xPtInterval;
            m_YPtInterval += yPtInterval;
            m_OldPos = pos;
        }
    }
    p_End = event->pos();
    this->update();
}

void Cfhs_SavePoint::mouseReleaseEvent(QMouseEvent * event)
{
    m_Pressed = false;
    if (event->button() == Qt::LeftButton) {
        switch (select) {
        case 0:
            //拖动事件，不做任何处理
            break;
        case 1:
            //记录矩形，需要根据给定的坐标按比例在原图上进行绘画
            SaveRect();
            break;
        case 2:
            //记录圆形，同上
            SaveEllipse();
            break;
        default:
            break;

        }
    }
    this->update();
}

void Cfhs_SavePoint::ClickButton_rect()
{
    select = 1;
}

void Cfhs_SavePoint::ClickButton_elli()
{
    select = 2;
}

void Cfhs_SavePoint::ClickButton_line()
{
    select = 0;
}

void Cfhs_SavePoint::ClickButton_move()
{
    select = 0;
}

void Cfhs_SavePoint::ClickButton_del()
{
    select = 4;
}
