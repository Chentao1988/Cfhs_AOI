#include "img_show_managent.h"
#include <QMessageBox>

#pragma execution_character_set("utf-8")


Img_Show_Managent::Img_Show_Managent(QWidget *parent) :
    QDialog(parent)
{
    //算法执行
    algorithm_open_button = new switch_button(tr("算法禁用"), tr("算法启用"), this);
    connect(algorithm_open_button, &switch_button::sig_button_clicked,
            this, &Img_Show_Managent::algorithm_open_button_clicked);
    QHBoxLayout *algorithmLayout = new QHBoxLayout;
    algorithmLayout->addStretch();
    algorithmLayout->addWidget(algorithm_open_button);
    algorithmLayout->addStretch();
    //工位图来源
    station_open_button = new switch_button(tr("工位图静态"),tr("工位图动态"),this);
    connect(station_open_button, &switch_button::sig_button_clicked,
            this, &Img_Show_Managent::station_open_button_clicked);
    QHBoxLayout *stationLayout = new QHBoxLayout;
    stationLayout->addStretch();
    stationLayout->addWidget(station_open_button);
    stationLayout->addStretch();
    //button init
    button_yes = new QPushButton(this);
    button_yes->setDefault(true);
    button_yes->setText(tr("确定"));
    connect(button_yes, &QPushButton::clicked,
            this, &Img_Show_Managent::button_yes_click);
    button_no = new QPushButton(this);
    button_no->setText(tr("取消"));
    connect(button_no, &QPushButton::clicked,
            this, &Img_Show_Managent::button_no_click);
    QHBoxLayout *layout_button = new QHBoxLayout();
    layout_button->addStretch();
    layout_button->addWidget(button_no);
    layout_button->addStretch();
    layout_button->addWidget(button_yes);
    layout_button->addStretch();
    //总layout
    QVBoxLayout *layout_all = new QVBoxLayout();
    layout_all->addLayout(algorithmLayout);
    layout_all->addLayout(stationLayout);
    layout_all->addLayout(layout_button);
    layout_all->setContentsMargins(9,9,9,20);
    layout_all->setSpacing(20);
    setLayout(layout_all);
    resize(400,220);
    this->setWindowTitle(tr("图像显示管理"));
}

Img_Show_Managent::~Img_Show_Managent()
{

}

void Img_Show_Managent::DialogShow()
{
    //获取方案
    QString strInfo;
    if(!m_logicInterface->GetConfigInfo(stConf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //算法执行
    algorithm_open_button->Set_Switch(stConf.bSaveImg);
    //工位图来源
    station_open_button->Set_Switch(stConf.bSaveImg);
    this->exec();
}

void Img_Show_Managent::button_yes_click()
{
    QString strInfo; //日志
    //设置方案信息
    bool isAgorithmUsed = algorithm_open_button->Get_Switch();
    bool isStationSys = station_open_button->Get_Switch();
    stConf.bSaveImg = isAgorithmUsed;
    stConf.bSysLang = isStationSys;
    if(!m_logicInterface->SetConfigInfo(stConf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QMessageBox::information(this, " ", tr("设置成功"));
    this->accept();
}

void Img_Show_Managent::button_no_click()
{
    this->reject();
}

void Img_Show_Managent::algorithm_open_button_clicked()
{
    bool isOpen = algorithm_open_button->Get_Switch();
    isOpen = !isOpen;
    algorithm_open_button->Set_Switch(isOpen);
}

void Img_Show_Managent::station_open_button_clicked()
{
    bool isOpen = station_open_button->Get_Switch();
    isOpen = !isOpen;
    station_open_button->Set_Switch(isOpen);
}
