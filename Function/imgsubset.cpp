#include "imgsubset.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>


#pragma execution_character_set("utf-8")

ImgSubSet::ImgSubSet(QWidget *parent)
    :QDialog(parent)
{
    resize(600,440);
    this->setWindowTitle(tr("图像分区设置"));
    QVBoxLayout *layout_All = new QVBoxLayout();
    QHBoxLayout *layout_EditAndButton = new QHBoxLayout();
    QVBoxLayout *layout_left_img = new QVBoxLayout();
    QVBoxLayout *layout_Edit = new QVBoxLayout();
    QHBoxLayout *layout_buttom = new QHBoxLayout();
    QHBoxLayout *layout_orbutton = new QHBoxLayout();

    button_cancel = new QPushButton(this);
    connect(button_cancel, &QPushButton::clicked,
            this, &ImgSubSet::button_cancel_clicked);
    button_detemine = new QPushButton(this);
    connect(button_detemine, &QPushButton::clicked,
            this, &ImgSubSet::button_detemine_clicked);

    button_cancel->setText(tr("取消"));
    button_detemine->setText(tr("确定"));
    layout_buttom->addWidget(button_cancel,0,Qt::AlignCenter);
    layout_buttom->addWidget(button_detemine,0,Qt::AlignCenter);

    edit_PointA = new Edit_Double(this);
    edit_PointD = new Edit_Double(this);
    edit_Partition = new Edit_Double(this);

    edit_PointA->Set_Left_Label(tr("A点坐标X:"));
    edit_PointA->Set_Right_Label(tr("A点坐标Y:"));
    edit_PointD->Set_Left_Label(tr("D点坐标X:"));
    edit_PointD->Set_Right_Label(tr("D点坐标Y:"));
    edit_Partition->Set_Left_Label(tr("分区行数:"));
    edit_Partition->Set_Right_Label(tr("分区列数:"));
    edit_Partition->SetText("3","3");

    layout_Edit->addWidget(edit_PointA,0,Qt::AlignCenter);
    layout_Edit->addWidget(edit_PointD,0,Qt::AlignCenter);
    layout_Edit->addWidget(edit_Partition,0,Qt::AlignCenter);

    negate_button = new switch_button(tr("反向"),tr("正向"),this);
    connect(negate_button, &switch_button::sig_button_clicked,
            this, &ImgSubSet::negate_button_clicked);
    open_button = new switch_button(tr("关闭"),tr("开启"),this);
    connect(open_button, &switch_button::sig_button_clicked,
            this, &ImgSubSet::open_button_clicked);


    layout_orbutton->addWidget(negate_button,0,Qt::AlignCenter);
    layout_orbutton->addWidget(open_button,0,Qt::AlignCenter);

    QLabel *img_label = new QLabel(this);
    QPixmap map;
    if(map.load(":/demo_grid_view.png")){
        img_label->setPixmap(map);
    }
    layout_left_img->addWidget(img_label, 0, Qt::AlignLeft);
    layout_Edit->addLayout(layout_orbutton);
    layout_EditAndButton->addLayout(layout_left_img);
    layout_EditAndButton->addLayout(layout_Edit);
    layout_All->addLayout(layout_EditAndButton);
    layout_All->addLayout(layout_buttom);
    layout_All->setContentsMargins(9,9,9,20);
    setLayout(layout_All);
}


ImgSubSet::~ImgSubSet()
{
}

void ImgSubSet::DialogShow()
{
    //获取当前数据
    QString strInfo;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, false, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QString strConfig = stPro.strImagePart;
    if(!setConfigPara(strConfig))
    {
        //使用默认值
        this->DataInit();
    }

    this->exec();
}

void ImgSubSet::Set_Point(QPoint left_top, QPoint right_bottom, bool flg_alphabet)
{
    edit_PointA->Set_Point(left_top);
    edit_PointD->Set_Point(right_bottom);
    negate_button->Set_Switch(flg_alphabet);
    pointA = left_top;
    pointD = right_bottom;
    imageNegate = flg_alphabet;
}

void ImgSubSet::Set_Grid_View(bool open_gridview, int x_gridview, int y_gridview)
{
    QPoint point;
    point.setX(x_gridview);
    point.setY(y_gridview);
    edit_Partition->Set_Point(point);
    open_button->Set_Switch(open_gridview);
    linePot = point;
    funcOpen = open_gridview;
}

void ImgSubSet::DataInit()
{
    Set_Point(QPoint(0,0), QPoint(0,0), false);
    Set_Grid_View(false ,3, 3);
}

bool ImgSubSet::setConfigPara(const QString &strConfig)
{
    if(strConfig.isEmpty()
            ||strConfig == "null")
        return false;
    QByteArray json;
    json.append(strConfig);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if(error.error != QJsonParseError::NoError)
        return false;
    if(!doc.isObject())
        return false;
    QJsonObject obj = doc.object();
    //A点坐标
    int ax = obj.take("Ax").toInt();
    int ay = obj.take("Ay").toInt();
    pointA.setX(ax);
    pointA.setY(ay);
    //D点坐标
    int dx = obj.take("Dx").toInt();
    int dy = obj.take("Dy").toInt();
    pointD.setX(dx);
    pointD.setY(dy);
    //行列数
    int row = obj.take("Row").toInt();
    int col = obj.take("Column").toInt();
    //qDebug()<<__FUNCTION__<<row<<","<<col;
    linePot.setX(row);
    linePot.setY(col);
    //取反
    imageNegate = obj.take("Forward").toBool();
    //开启
    funcOpen = obj.take("Open").toBool();
    //设置参数
    Set_Point(pointA, pointD, imageNegate);
    Set_Grid_View(funcOpen, row, col);

    return true;
}

const QString ImgSubSet::getConfigPara()
{
    QJsonObject obj;
    obj.insert("Ax", pointA.x());
    obj.insert("Ay", pointA.y());
    obj.insert("Dx", pointD.x());
    obj.insert("Dy", pointD.y());
    obj.insert("Row", linePot.x());
    obj.insert("Column", linePot.y());
    obj.insert("Forward", imageNegate);
    obj.insert("Open", funcOpen);
    QJsonDocument doc;
    doc.setObject(obj);

    return QString(doc.toJson(QJsonDocument::Compact));
}

void ImgSubSet::SetDataUpdate(const QString &strConfig)
{
    if(!setConfigPara(strConfig))
        DataInit();
    //设置坐标点
    emit sig_setPoint(pointA, pointD, imageNegate);
    //设置行列数
    emit sig_setGridView(funcOpen, linePot.x(), linePot.y());
}

void ImgSubSet::button_cancel_clicked()
{
    this->reject();
}

void ImgSubSet::button_detemine_clicked()
{
    //坐标点
    edit_PointA->Get_Point(pointA);
    edit_PointD->Get_Point(pointD);
    if(pointD.x()>=pointA.x()
            && pointD.y()>=pointA.y())
    {
        //功能开关
        funcOpen = open_button->Get_Switch();
        //图片取反
        imageNegate = negate_button->Get_Switch();
        //行列数
        edit_Partition->Get_Point(linePot);
        //设置坐标点
        emit sig_setPoint(pointA, pointD, imageNegate);
        //设置行列数
        emit sig_setGridView(funcOpen, linePot.x(), linePot.y());
        //保存到数据库
        QString strConfig = getConfigPara();
        stPro.strImagePart = strConfig;
        QString strInfo;
        if(!m_logicInterface->SetProInfo(m_curProgramName, stPro, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        this->accept();
    }
    else
        QMessageBox::warning(this, " ", tr("D点坐标不能小于A点坐标"));
}

void ImgSubSet::negate_button_clicked()
{
    bool isStatus = negate_button->Get_Switch();
    isStatus = !isStatus;
    negate_button->Set_Switch(isStatus);
}

void ImgSubSet::open_button_clicked()
{
    bool isOpen = open_button->Get_Switch();
    isOpen = !isOpen;
    open_button->Set_Switch(isOpen);
}

