#include "cfhs_imagewindow.h"
#include <QTime>
#include <QBitmap>

Cfhs_ImageWindow::Cfhs_ImageWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(800,600);
    this->init();
    this->setWindowStyle();

}

Cfhs_ImageWindow::~Cfhs_ImageWindow()
{

}

QString Cfhs_ImageWindow::getFunctionButtonStyle(const QString &name)
{
    QString style = QString("QPushButton{image:url(:/%1_normal.png);"
                            "background:transparent; border:none;}"
                            "QPushButton:pressed{image:url(:/%2_press.png)}"
                            "QPushButton:checked{image:url(:/%3_press.png);"
                            "border: 1px solid red}").arg(name).arg(name).arg(name);

    return style;
}

//载入图片
void Cfhs_ImageWindow::setImage(const QString &path)
{
    QTime time;
    time.start();
    QFile file(path);
    if (!file.exists())
    {
        save_map = QPixmap();
        this->update();
        return;
    }
    //Qt::ImageConversionFlags flag;
    save_map.load(path);
    qDebug()<<__FUNCTION__<<"pixmap,"<<time.elapsed();
    int width = qMin(save_map.width(), this->width());
    int height = static_cast<int>(width * 1.0 / (save_map.width() * 1.0 / save_map.height()));
    height = qMin(height, this->height());
    width = static_cast<int>(height * 1.0 * (save_map.width() * 1.0 / save_map.height()));
    save_width = width;
    save_height = height;
    this->update();
}

void Cfhs_ImageWindow::getAllPoint(QMap<int, QList<QPoint>> &map)
{
    savemap_point[0] = list_thistoimg(Rect_start_save_point);
    savemap_point[1] = list_thistoimg(Rect_end_save_point);
    savemap_point[2] = list_thistoimg(Ellipse_start_save_point);
    savemap_point[3] = list_thistoimg(Ellipse_end_save_point);
    savemap_point[4] = list_thistoimg(Line_save_point);
    map = savemap_point;
}

void Cfhs_ImageWindow::getAllPoint(QMap<int, QList<QList<QPoint>>> &map)
{
    map.clear();
    QList<QList<QPoint>> rectArea;
    for(int i=0; i<Rect_end_save_point.size();i++) {
        QList<QPoint> rectSig;
        rectSig.append(list_thistoimg(Rect_start_save_point).at(i));
        rectSig.append(list_thistoimg(Rect_end_save_point).at(i));
        rectArea.append(rectSig);
    }
    QList<QList<QPoint>> ellipseArea;
    for(int i=0; i<Ellipse_start_save_point.size();i++) {
        QList<QPoint> ellipseSig;
        ellipseSig.append(list_thistoimg(Ellipse_start_save_point).at(i));
        ellipseSig.append(list_thistoimg(Ellipse_end_save_point).at(i));
        ellipseArea.append(ellipseSig);
    }
    map.insert(0,rectArea);
    map.insert(1,ellipseArea);
   //QList<QList<QPoint>> lineArea;
   //lineArea.append(list_thistoimg(Line_save_point));
   //map.insert(2,lineArea);
    /*
    QMap<int, QList<QList<QPoint> > > map1;
    map1[0][0] = list_thistoimg(Rect_start_save_point);
    map1[0][1] = list_thistoimg(Rect_end_save_point);
    map1[1][0] = list_thistoimg(Ellipse_start_save_point);
    map1[1][1] = list_thistoimg(Ellipse_end_save_point);
    map1[2][0] = list_thistoimg(Line_save_point);*/
}

void Cfhs_ImageWindow::loadPoint(QMap<int, QList<QPoint> > getmap)
{
    Rect_start_save_point = list_thistoimg(getmap[0]);
    Rect_end_save_point = list_thistoimg(getmap[1]);
    Ellipse_start_save_point = list_thistoimg(getmap[2]);
    Ellipse_end_save_point = list_thistoimg(getmap[3]);
    Line_save_point = list_thistoimg(getmap[4]);
}

void Cfhs_ImageWindow::loadPoint(QMap<int, QList<QList<QPoint> > > getmap)
{

    if(!getmap[0].isEmpty()){
        Rect_start_save_point = list_imgtothis(getmap[0][0]);
        Rect_end_save_point = list_imgtothis(getmap[0][1]);
    }
    if(!getmap[1].isEmpty()){
        Ellipse_start_save_point = list_imgtothis(getmap[1][0]);
        Ellipse_end_save_point = list_imgtothis(getmap[1][1]);
    }
    if(!getmap[2].isEmpty()){
        Line_save_point = list_imgtothis(getmap[2][0]);
    }
//    Line_save_point = list_imgtothis(map[0][4]);
}
//保存函数
bool Cfhs_ImageWindow::saveImage(QString savepath)
{
    if(save_map.save(savepath)){
        return true;
    }
    return false;
}

QPushButton *Cfhs_ImageWindow::getButton(const QString &name)
{
    QPushButton *button = new QPushButton(this);
    button->setFixedSize(35, 35);
    QString style = getFunctionButtonStyle(name);
    button->setStyleSheet(style);
    button->setCheckable(true);
    pLayout->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);
    pButtonGroup->addButton(button);

    return button;
}

/*
QList<QPoint> Cfhs_ImageWindow::list_thistoimg(QList<QPoint> listpoint)
{
    QList<QPoint> list_return;
    QPoint point_temp;
    for (int i=0;i<listpoint.size();i++) {
        int x = (listpoint[i].x() )*save_width/save_map.width()+ (this->width() - save_width)/2;
        int y = (listpoint[i].y() )*save_height/save_map.height()+ (this->height() - save_height)/2;
        point_temp.setX(x);
        point_temp.setY(y);
        list_return<<point_temp;
    }
    return list_return;
}

QList<QPoint> Cfhs_ImageWindow::list_imgtothis(QList<QPoint> listpoint)
{
    QList<QPoint> list_return;
    for (int i=0;i<listpoint.size();i++) {
        int x = (listpoint[i].x()+ (this->width() - save_width)/2)*save_width/save_map.width();
        int y = (listpoint[i].y()+ (this->height() - save_height)/2)*save_height/save_map.height();
        list_return[i].setX(x);
        list_return[i].setY(y);
    }
    return list_return;
}
*/
QList<QPoint> Cfhs_ImageWindow::list_imgtothis(QList<QPoint> listpoint)
{
    QList<QPoint> list_return;
    if(save_map.isNull())
        return list_return;
    QPoint point_temp;
    for (int i=0;i<listpoint.size();i++) {
        int x_len = (this->width() - save_width)/2;
        int y_len = (this->height() - save_height)/2;
        int x = int((listpoint[i].x() )*save_width/save_map.width()+0.5 + x_len);
        int y = int((listpoint[i].y() )*save_height/save_map.height()+0.5+ y_len);
        point_temp.setX(x);
        point_temp.setY(y);
        list_return<<point_temp;
    }
    return list_return;
}

QList<QPoint> Cfhs_ImageWindow::list_thistoimg(QList<QPoint> listpoint)
{
    QList<QPoint> list_return;
    if(save_map.isNull())
        return list_return;
    QPoint point_temp;
    for (int i=0;i<listpoint.size();i++) {
        int x_len = (this->width() - save_width)/2;
        int y_len = (this->height() - save_height)/2;
        int x = (listpoint[i].x()- x_len)*save_map.width()/save_width;
        int y = (listpoint[i].y()- y_len)*save_map.height()/save_height;
        point_temp.setX(x);
        point_temp.setY(y);
        list_return<<point_temp;
    }
    return list_return;
}

//鼠标右键
void Cfhs_ImageWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();
    p_end = pos;
}

//  绘制函数

void Cfhs_ImageWindow::paintEvent(QPaintEvent *event)
{
    // 绘制样式
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    if (save_map.isNull())
        return QWidget::paintEvent(event);

    // 根据窗口计算应该显示的图片的大小
    int width = qMin(save_map.width(), this->width());
    int height = static_cast<int>(width * 1.0 / (save_map.width() * 1.0 / save_map.height()));
    height = qMin(height, this->height());
    width = static_cast<int>(height * 1.0 * (save_map.width() * 1.0 / save_map.height()));
    save_width = width;
    save_height = height;
    /*记录 长宽缩小比率，获取鼠标坐标使用 */

    int xPos = this->width()/2 + m_XPtInterval;
    int yPos = this->height()/2 + m_YPtInterval;
    // 平移
    painter.translate(xPos, yPos);
    // 缩放
    painter.scale(m_ZoomValue, m_ZoomValue);
    // 绘制图像
    QRect picRect(-width / 2, -height / 2, width, height);
    painter.drawPixmap(picRect, save_map);
    if(move_flg != false){
        switch(select){
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
    Crect_map(xPos,yPos);
    CdrawEllipse_map(xPos,yPos);
//    CLine_map(xPos,yPos);

}
//鼠标滚轮
void Cfhs_ImageWindow::wheelEvent(QWheelEvent *event)
{
    center_Point = event->pos();
    int value = event->delta();
    if (value > 0)
        onZoomInImage();
    else
        onZoomOutImage();
    select = 0;
    this->update();
}
//鼠标按下
void Cfhs_ImageWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        move_flg = true;
        m_OldPos = event->pos();
        p_start = event->pos();
        p_end = event->pos();
        m_Pressed = true;

        int Px = event->pos().x()*save_map.height()/this->height();
        point_show.setX(Px);
        int Py = event->pos().y()*save_map.width()/this->width();
        point_show.setY(Py);
        if(select == 4)
        {
            delectpoint(m_OldPos.x(),m_OldPos.y());
        }
    }
    this->update();
}
//鼠标移动
void Cfhs_ImageWindow::mouseMoveEvent(QMouseEvent *event)
{

    if(select == 0){
        if (!m_Pressed)
            return QWidget::mouseMoveEvent(event);

        this->setCursor(Qt::ClosedHandCursor);
        QPoint pos = event->pos();
        int xPtInterval = pos.x() - m_OldPos.x();
        int yPtInterval = pos.y() - m_OldPos.y();
        m_XPtInterval += xPtInterval;
        m_YPtInterval += yPtInterval;
        m_OldPos = pos;
    }
    p_end = event->pos();
    if(select!=3)
        this->update();
}
//鼠标释放
void Cfhs_ImageWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_Pressed = false;
        move_flg = false;
        switch (select){
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
        case 3:
            //根据多点进行连接，根据多点的坐标用lineto连接，最后一点连接第一点
            SaveLine();
            break;
        default:
            break;

        }
    }
    this->setCursor(Qt::ArrowCursor);
    QWidget::mouseReleaseEvent(event);
}

QAction *Cfhs_ImageWindow::getAction(const QString& imagePath, const QString& title)
{
    QAction *action = new QAction(this);
    QPixmap map;
    map.load(imagePath);
    map = map.scaled(32,32,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    QIcon icon;
    icon.addPixmap(map);
    action->setIcon(icon);
    action->setText(title);
    action->setIconText(title);

    return action;
}
//背景颜色设置
void Cfhs_ImageWindow::setWindowStyle()
{
    //界面背景色
    this->setStyleSheet("Cfhs_ImageWindow{background:#000A19}");
}
//初始化函数
void Cfhs_ImageWindow::init()
{
    pLayout = new QHBoxLayout();
    pButtonGroup = new QButtonGroup(this);
    button_zoomin = getButton("zoom_in");
    button_zoomout = getButton("zoom_out");
    button_adaptive = getButton("adaptive");
    button_rect = getButton("rectangle");
    button_elli = getButton("circle");
    button_line = getButton("polygon");
    button_save = getButton("load");
    button_del = getButton("delete");
    connect(button_zoomin,SIGNAL(clicked()),this,SLOT(ClickButton_zoomin()));
    connect(button_zoomout,SIGNAL(clicked()),this,SLOT(ClickButton_zoomout()));
    connect(button_adaptive,SIGNAL(clicked()),this,SLOT(ClickButton_adaptive()));
    connect(button_rect,SIGNAL(clicked()),this,SLOT(ClickButton_rect()));
    connect(button_elli,SIGNAL(clicked()),this,SLOT(ClickButton_elli()));
    connect(button_line,SIGNAL(clicked()),this,SLOT(ClickButton_line()));
    connect(button_save,SIGNAL(clicked()),this,SLOT(ClickButton_save()));
    connect(button_del,SIGNAL(clicked()),this,SLOT(ClickButton_del()));

    pLayout->addStretch();
    pLayout->setSpacing(10);
    this->setLayout(pLayout);
    //value init
    m_ZoomValue = 1.0;
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    point_start = QPoint(0,0);
    select = 0;
    m_Pressed = false;
    move_flg = false;
}

//画矩形
void Cfhs_ImageWindow::CRect()
{
    QPainter painter(this);
    painter.setPen(Qt::red);//设置画笔颜色
    QRect temp(p_start,p_end);
    painter.drawRect(temp);
}
void Cfhs_ImageWindow::SaveRect()
{
    QPoint st_point;
    QPoint se_point;
    st_point.setX(p_start.x()-m_XPtInterval);
    st_point.setY(p_start.y()-m_YPtInterval);
    se_point.setX(p_end.x()-m_XPtInterval);
    se_point.setY(p_end.y()-m_YPtInterval);
    st_point.setX(int(this->width()/2-(this->width()/2 - st_point.x())/m_ZoomValue));
    st_point.setY(int(this->height()/2-(this->height()/2 - st_point.y())/m_ZoomValue));
    se_point.setX(int(this->width()/2-(this->width()/2 - se_point.x())/m_ZoomValue));
    se_point.setY(int(this->height()/2-(this->height()/2 - se_point.y())/m_ZoomValue));
    Rect_start_save_point<<st_point;
    Rect_end_save_point<<se_point;
}
void Cfhs_ImageWindow::Crect_map(int xpos,int ypos)
{

    for (int i = 0;i<Rect_start_save_point.size();i++) {
        int st_x,st_y;
        st_x = Rect_start_save_point[i].x() + m_XPtInterval;
        st_y = Rect_start_save_point[i].y() + m_YPtInterval;
        QPainter painter(this);
        painter.setPen(Qt::red);//设置画笔颜色
        QRect temp(st_x,st_y,Rect_end_save_point[i].x()-Rect_start_save_point[i].x(),
                   Rect_end_save_point[i].y()-Rect_start_save_point[i].y());
        painter.translate(xpos, ypos);
        painter.scale(m_ZoomValue,m_ZoomValue);
        painter.translate(-xpos, -ypos);
        painter.drawRect(temp);//画矩形
    }
}


//画圆形
void Cfhs_ImageWindow::CdrawEllipse()
{


    QPainter painter(this);
    painter.setPen(Qt::red);//设置画笔颜色
    QRect temp(p_start, p_end);
    painter.drawEllipse(temp);//画圆
}
void Cfhs_ImageWindow::SaveEllipse()
{
    QPoint st_point;
    QPoint se_point;
    st_point.setX(p_start.x()-m_XPtInterval);
    st_point.setY(p_start.y()-m_YPtInterval);
    se_point.setX(p_end.x()-m_XPtInterval);
    se_point.setY(p_end.y()-m_YPtInterval);
    st_point.setX(int(this->width()/2-(this->width()/2 - st_point.x())/m_ZoomValue));
    st_point.setY(int(this->height()/2-(this->height()/2 - st_point.y())/m_ZoomValue));
    se_point.setX(int(this->width()/2-(this->width()/2 - se_point.x())/m_ZoomValue));
    se_point.setY(int(this->height()/2-(this->height()/2 - se_point.y())/m_ZoomValue));
    Ellipse_start_save_point<<st_point;
    Ellipse_end_save_point<<se_point;
}
void Cfhs_ImageWindow::CdrawEllipse_map(int xpos, int ypos)
{

    for (int i = 0;i<Ellipse_start_save_point.size();i++){
        int st_x,st_y;
        st_x = Ellipse_start_save_point[i].x() + m_XPtInterval;
        st_y = Ellipse_start_save_point[i].y() + m_YPtInterval;
        QRect temp(st_x,st_y, Ellipse_end_save_point[i].x()-Ellipse_start_save_point[i].x(),
                   Ellipse_end_save_point[i].y()-Ellipse_start_save_point[i].y());
        QPainter painter(this);
        painter.setPen(Qt::red);//设置画笔颜色
        painter.translate(xpos, ypos);
        painter.scale(m_ZoomValue,m_ZoomValue);
        painter.translate(-xpos, -ypos);
        painter.drawEllipse(temp);//画圆
    }

}


void Cfhs_ImageWindow::CLineTo(int x, int y)
{
    if(point_start.x() == 0 &&point_start.y() == 0)
    {
        point_start.setX(x);
        point_start.setY(y);
    }
    else {
        QPainter painter(this);
        painter.setPen(Qt::red);//设置画笔颜色
        painter.drawLine(point_start,p_start);
        point_start = p_start;
    }

}

void Cfhs_ImageWindow::CLine_map(int xpos,int ypos)
{
    if(Line_save_point.size()<2){
        return;
    }
    for (int i = 0;i<Line_save_point.size();i++){
        int st_x,st_y,se_x,se_y;
        st_x = Line_save_point[i].x() + m_XPtInterval;
        st_y = Line_save_point[i].y() + m_YPtInterval;
        if(i+1!=Line_save_point.size()){
            se_x = Line_save_point[i+1].x() + m_XPtInterval;
            se_y = Line_save_point[i+1].y() + m_YPtInterval;
            QPainter painter(this);
            painter.setPen(Qt::red);//设置画笔颜色
            painter.translate(xpos, ypos);
            painter.scale(m_ZoomValue,m_ZoomValue);
            painter.translate(-xpos, -ypos);
            painter.drawLine(st_x,st_y,se_x,se_y);//画线
        }
        else {
            se_x = Line_save_point[0].x() + m_XPtInterval;
            se_y = Line_save_point[0].y() + m_YPtInterval;
            QPainter painter(this);
            painter.setPen(Qt::red);//设置画笔颜色
            painter.translate(xpos, ypos);
            painter.scale(m_ZoomValue,m_ZoomValue);
            painter.translate(-xpos, -ypos);
            painter.drawLine(st_x,st_y,se_x,se_y);//画线
        }
    }
}

void Cfhs_ImageWindow::SaveLine()
{
    QPoint point1;
    point1.setX(p_start.x()-m_XPtInterval);
    point1.setY(p_start.y()-m_YPtInterval);
    point1.setX(int(this->width()/2-(this->width()/2 - point1.x())/m_ZoomValue));
    point1.setY(int(this->height()/2-(this->height()/2 - point1.y())/m_ZoomValue));
    Line_save_point<<point1;
    this->update();
}

void Cfhs_ImageWindow::delectpoint(int x, int y)
{
    int point_x = x-m_XPtInterval;
    point_x = int(this->width()/2-(this->width()/2 - point_x)/m_ZoomValue);
    int point_y = y-m_YPtInterval;
    point_y = int(this->height()/2-(this->height()/2 - point_y)/m_ZoomValue);
    for (int i=0;i<Rect_start_save_point.size();i++) {
        if(point_x>Rect_start_save_point[i].x()&&point_y>Rect_start_save_point[i].y()&&
                point_x<(Rect_end_save_point[i].x())&&
                point_y<(Rect_end_save_point[i].y())) {
            Rect_start_save_point.removeAt(i);
            Rect_end_save_point.removeAt(i);
        }
    }
    for (int i=0;i<Rect_start_save_point.size();i++) {
        if(point_x>Rect_end_save_point[i].x()&&point_y>Rect_end_save_point[i].y()&&
                point_x<(Rect_start_save_point[i].x())&&
                point_y<(Rect_start_save_point[i].y())) {
            Rect_start_save_point.removeAt(i);
            Rect_end_save_point.removeAt(i);
        }
    }
    for (int i=0;i<Ellipse_start_save_point.size();i++) {
        if(point_x>Ellipse_start_save_point[i].x()&&point_y>Ellipse_start_save_point[i].y()&&
                point_x<(Ellipse_end_save_point[i].x())&&
                point_y<(Ellipse_end_save_point[i].y())){
            Ellipse_start_save_point.removeAt(i);
            Ellipse_end_save_point.removeAt(i);
        }
    }
    for (int i=0;i<Ellipse_start_save_point.size();i++) {
        if(point_x>Ellipse_end_save_point[i].x()&&point_y>Ellipse_end_save_point[i].y()&&
                point_x<(Ellipse_start_save_point[i].x())&&
                point_y<(Ellipse_start_save_point[i].y())) {
            Ellipse_start_save_point.removeAt(i);
            Ellipse_end_save_point.removeAt(i);
        }
    }
}

//鼠标滚轮
void Cfhs_ImageWindow::onZoomInImage(void)
{
    m_ZoomValue *=1.1;
    this->update();
}
//鼠标滚轮
void Cfhs_ImageWindow::onZoomOutImage(void)
{
    m_ZoomValue /= 1.1;
    if (m_ZoomValue < 0.2)
        m_ZoomValue = 0.2;

    this->update();
}

void Cfhs_ImageWindow::onPresetImage(void)
{
    m_ZoomValue = 1.0;
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    this->update();
}

void Cfhs_ImageWindow::ClickButton_rect()
{
    select = 1;
}

void Cfhs_ImageWindow::ClickButton_elli()
{
    select = 2;
}

void Cfhs_ImageWindow::ClickButton_line()
{
    select = 3;
}

void Cfhs_ImageWindow::ClickButton_save()
{
    select = 0;
    QMap<int, QList<QList<QPoint>>> map;
    getAllPoint(map);
    if(!map.isEmpty())
        emit sig_sendRoiPoint(map);
}

void Cfhs_ImageWindow::ClickButton_del()
{
    select = 4;
}

void Cfhs_ImageWindow::ClickButton_zoomin()
{
    onZoomInImage();
}

void Cfhs_ImageWindow::ClickButton_zoomout()
{
    onZoomOutImage();
}

void Cfhs_ImageWindow::ClickButton_adaptive()
{
    m_ZoomValue = 1.0;
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    this->update();
}
