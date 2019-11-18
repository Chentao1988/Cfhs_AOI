#include "imgsubset.h"
#include <QMessageBox>

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
    edit_PointB = new Edit_Double(this);
    edit_PointC = new Edit_Double(this);
    edit_PointD = new Edit_Double(this);
    edit_Partition = new Edit_Double(this);

    edit_PointA->Set_Left_Label(tr("A点坐标X:"));
    edit_PointA->Set_Right_Label(tr("A点坐标Y:"));
    edit_PointB->Set_Left_Label(tr("B点坐标X:"));
    edit_PointB->Set_Right_Label(tr("B点坐标Y:"));
    edit_PointC->Set_Left_Label(tr("C点坐标X:"));
    edit_PointC->Set_Right_Label(tr("C点坐标Y:"));
    edit_PointD->Set_Left_Label(tr("D点坐标X:"));
    edit_PointD->Set_Right_Label(tr("D点坐标Y:"));
    edit_Partition->Set_Left_Label(tr("分区行数:"));
    edit_Partition->Set_Right_Label(tr("分区列数:"));
    edit_Partition->SetText("3","3");

    layout_Edit->addWidget(edit_PointA,0,Qt::AlignCenter);
    layout_Edit->addWidget(edit_PointB,0,Qt::AlignCenter);
    layout_Edit->addWidget(edit_PointC,0,Qt::AlignCenter);
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

void ImgSubSet::Set_Point(QPoint left_top, QPoint right_top,
                          QPoint left_bottom, QPoint right_bottom, bool flg_alphabet)
{
    edit_PointA->Set_Point(left_top);
    edit_PointB->Set_Point(right_top);
    edit_PointC->Set_Point(left_bottom);
    edit_PointD->Set_Point(right_bottom);
    negate_button->Set_Switch(flg_alphabet);
}

void ImgSubSet::Set_Grid_View(bool open_gridview, int x_gridview, int y_gridview)
{
    QPoint point;
    point.setX(x_gridview);
    point.setY(y_gridview);
    edit_Partition->Set_Point(point);
    open_button->Set_Switch(open_gridview);
}

void ImgSubSet::button_cancel_clicked()
{
    this->reject();
}

void ImgSubSet::button_detemine_clicked()
{
    //四个坐标点
    QPoint pointA;
    QPoint pointB;
    QPoint pointC;
    QPoint pointD;
    edit_PointA->Get_Point(pointA);
    edit_PointB->Get_Point(pointB);
    edit_PointC->Get_Point(pointC);
    edit_PointD->Get_Point(pointD);
    if(pointA.y() == pointB.y()
            && pointA.x() == pointC.x()
            && pointD.x() == pointB.x()
            && pointD.y() == pointC.y())
    {
        //功能开关
        bool funcOpen = open_button->Get_Switch();
        //图片取反
        bool imageNegate = negate_button->Get_Switch();
        //行列数
        QPoint linePot;
        edit_Partition->Get_Point(linePot);
        //设置坐标点
        emit sig_setPoint(pointA, pointB, pointC, pointD, imageNegate);
        //设置行列数
        emit sig_setGridView(funcOpen, linePot.x(), linePot.y());
        this->accept();
    }
    else
    {
        QMessageBox::warning(this, " ", tr("四个坐标点不是矩形，请重新设置"));
        return;
    }
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

