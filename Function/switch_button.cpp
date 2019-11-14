#include "switch_button.h"
#include <QCoreApplication>
#pragma execution_character_set("utf-8")

switch_button::switch_button(QString left, QString right  ,QWidget *parent)
    :QWidget(parent)
{
    resize(200,100);
    QHBoxLayout *layout_orbutton = new QHBoxLayout(this);
    label_left = new QLabel(this);
    button_center = new QPushButton(this);
    connect(button_center, &QPushButton::clicked,
            this, &switch_button::sig_button_clicked);
    label_right = new QLabel(this);
    label_left->setText(left);
    button_center->setFixedSize(75,34);
    label_right->setText(right);
    layout_orbutton->addWidget(label_left,0,Qt::AlignCenter);
    layout_orbutton->addWidget(button_center,0,Qt::AlignCenter);
    layout_orbutton->addWidget(label_right,0,Qt::AlignCenter);
    Set_Switch(false);
}

switch_button::~switch_button()
{

}

void switch_button::Set_Left_Label(QString left)
{
    label_left->setText(left);
}

void switch_button::Set_Right_Label(QString Right)
{
    label_right->setText(Right);
}

bool switch_button::Get_Switch() const
{
    return Flg_LeftOrRight;
}

void switch_button::Set_Switch(const bool &isOpen)
{
    Flg_LeftOrRight = isOpen;
    QString style = getSwitchButtonStyle(isOpen);
    button_center->setStyleSheet(style);
}

void switch_button::Set_Switch_Enable(const bool &isEnable)
{
    button_center->setEnabled(isEnable);
    if(!isEnable)
        Set_Switch(isEnable);
}

QString switch_button::getSwitchButtonStyle(const bool &isSwitch)
{
    QString style = "";
    if(isSwitch)
    {
        style = QString("QPushButton{border-image: url(:/button_check.png);"
                                     "background-color:transparent;border:none}"
                                     "QPushButton::disabled{background: #3954C9;"
                                     "border: 1px solid #3954C9;}");
    }
    else
    {
        style = QString("QPushButton{border-image: url(:/button_uncheck.png);"
                                     "background-color:transparent;border:none}"
                                     "QPushButton::disabled{background: #3954C9;"
                                     "border: 1px solid #3954C9;}");
    }

    return style;
}
