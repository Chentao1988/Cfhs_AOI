#include "edit_double.h"
#pragma execution_character_set("utf-8")

Edit_Double::Edit_Double(QWidget *parent)
    :QWidget(parent)
{
    resize(200,100);
    QHBoxLayout *layout_edit = new QHBoxLayout(this);
    label_left = new QLabel(this);
    label_right = new QLabel(this);
    Edit_Left = new QLineEdit(this);
    Edit_Right = new QLineEdit(this);
    Edit_Left->setValidator(new QIntValidator(0,100000,this));
    Edit_Right->setValidator(new QIntValidator(0,100000,this));
    layout_edit->addWidget(label_left);
    layout_edit->addWidget(Edit_Left);
    layout_edit->addWidget(label_right);
    layout_edit->addWidget(Edit_Right);
    Edit_Left->setText("0");
    Edit_Right->setText("0");
    setLayout(layout_edit);
    connect(Edit_Left,SIGNAL(selectionChanged()),this,SLOT(Change_Edit_Left()));
    connect(Edit_Right,SIGNAL(selectionChanged()),this,SLOT(Change_Edit_Right()));
}

Edit_Double::~Edit_Double()
{

}

void Edit_Double::Set_Left_Label(QString Left)
{
    label_left->setText(Left);
}

void Edit_Double::Set_Right_Label(QString Right)
{
    label_right->setText(Right);
}

void Edit_Double::Get_Point(QPoint &point)
{
    int xPos = Edit_Left->text().toInt();
    int yPos = Edit_Right->text().toInt();
    point.setX(xPos);
    point.setY(yPos);
}

void Edit_Double::Set_Point(const QPoint &point)
{
    Edit_Left->setText(QString::number(point.x()));
    Edit_Right->setText(QString::number(point.y()));
}

void Edit_Double::Change_Edit_Left()
{
    QString getdata = Edit_Left->displayText();
}

void Edit_Double::Change_Edit_Right()
{
    QString getdata = Edit_Right->displayText();
}

void Edit_Double::SetText(QString data1, QString data2)
{
    Edit_Left->setText(data1);
    Edit_Right->setText(data2);
}
