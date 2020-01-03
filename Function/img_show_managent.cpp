#include "img_show_managent.h"
#include "cfhs_imagetranscoding.h"
#include <QMessageBox>

#pragma execution_character_set("utf-8")


Img_Show_Managent::Img_Show_Managent(QWidget *parent) :
    QDialog(parent)
{
    //算法开关
    algorithm_open_button = new switch_button(tr("算法禁用"), tr("算法启用"), this);
    connect(algorithm_open_button, &switch_button::sig_button_clicked,
            this, &Img_Show_Managent::algorithm_open_button_clicked);
    //工位图来源
    station_open_button = new switch_button(tr("工位图静态"), tr("工位图动态"), this);
    connect(station_open_button, &switch_button::sig_button_clicked,
            this, &Img_Show_Managent::station_open_button_clicked);
    //抽样压缩
    sample_open_button = new switch_button(tr("抽样压缩关闭"), tr("抽样压缩开启"), this);
    connect(sample_open_button, &switch_button::sig_button_clicked,
            this, &Img_Show_Managent::sample_open_button_clicked);
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
    //静态图设置
    button_static_img = new QPushButton(tr("静态图设置"), this);
    connect(button_static_img, &QPushButton::clicked,
            this, &Img_Show_Managent::button_static_img_clicked);

    QHBoxLayout *layout_button = new QHBoxLayout();
    layout_button->addWidget(button_static_img, 0, Qt::AlignCenter);
    layout_button->addWidget(button_no, 0, Qt::AlignCenter);
    layout_button->addWidget(button_yes, 0, Qt::AlignCenter);
    //总layout
    QVBoxLayout *layout_all = new QVBoxLayout();
    layout_all->addWidget(algorithm_open_button, 0, Qt::AlignCenter);
    layout_all->addWidget(station_open_button, 0, Qt::AlignCenter);
    layout_all->addWidget(sample_open_button, 0, Qt::AlignCenter);
    layout_all->addLayout(layout_button);
    layout_all->setContentsMargins(9,9,9,20);
    layout_all->setSpacing(20);
    setLayout(layout_all);
    resize(460,240);
    this->setWindowTitle(tr("图像显示管理"));
}

Img_Show_Managent::~Img_Show_Managent()
{

}

int Img_Show_Managent::DialogShow()
{
    //获取方案
    QString strInfo;
    if(!m_logicInterface->GetConfigInfo(stConf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return QDialog::Rejected;
    }
    //算法执行
    algorithm_open_button->Set_Switch(stConf.bAlgorithmOnOrOff);
    //工位图来源
    station_open_button->Set_Switch(stConf.bStationDynamic);
    //抽样压缩
    sample_open_button->Set_Switch(stConf.bSamplingCompressedImg);
    //获取工位数
    QString strAllStation;
    if(!m_logicInterface->GetAllStationNo(m_curProgramName, strAllStation, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return QDialog::Rejected;
    }
    QStringList listStation = getListFromQString(strAllStation);
    station_num = listStation.size();
    return this->exec();
}

void Img_Show_Managent::button_yes_click()
{
    if(!Cfhs_ImageTranscoding::isStaticImgExists(station_num))
        return;
    QString strInfo; //日志
    //设置方案信息
    stConf.bAlgorithmOnOrOff = algorithm_open_button->Get_Switch();
    stConf.bStationDynamic = station_open_button->Get_Switch();
    stConf.bSamplingCompressedImg = sample_open_button->Get_Switch();
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

void Img_Show_Managent::sample_open_button_clicked()
{
    bool isOpen = sample_open_button->Get_Switch();
    isOpen = !isOpen;
    sample_open_button->Set_Switch(isOpen);
}

void Img_Show_Managent::station_open_button_clicked()
{
    bool isOpen = station_open_button->Get_Switch();
    isOpen = !isOpen;
    station_open_button->Set_Switch(isOpen);
}

void Img_Show_Managent::algorithm_open_button_clicked()
{
    bool isOpen = algorithm_open_button->Get_Switch();
    isOpen = !isOpen;
    algorithm_open_button->Set_Switch(isOpen);
}

void Img_Show_Managent::button_static_img_clicked()
{
    Cfhs_ImageTranscoding image(this, station_num);

    if(image.exec() == QDialog::Accepted)
        QMessageBox::information(this, " ", tr("设置成功"));
}
