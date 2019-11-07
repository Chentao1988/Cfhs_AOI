#include "img_show_managent.h"
#include "cfhs_global.h"
#include <QMessageBox>

#pragma execution_character_set("utf-8")


Img_Show_Managent::Img_Show_Managent(int number,QWidget *parent) :
    QDialog(parent)
{
    resize(600,400);
    this->setWindowTitle(tr("图像显示管理"));

    button_result = new lineedit_button(tr("结果图像路径"),this);
    button_analysis = new lineedit_button(tr("分析图像路径"),this);

    button_yes = new QPushButton(this);
    button_yes->setDefault(true);
    button_no = new QPushButton(this);

    result_open_button = new switch_button(tr("静态"),tr("动态"),this);
    station_open_button = new switch_button(tr("静态"),tr("动态"),this);
    QVBoxLayout *layout_all = new QVBoxLayout();
    QHBoxLayout *layout_comboBox_Up = new QHBoxLayout();
    QHBoxLayout *layout_comboBox_Down = new QHBoxLayout();
    QVBoxLayout *layout_all_comboBox = new QVBoxLayout();
    QHBoxLayout *layout_comboBox_swbutton = new QHBoxLayout();
    QHBoxLayout *layout_button = new QHBoxLayout();

    QLabel *result_label = new QLabel(this);
    QLabel *analysis_label = new QLabel(this);
    result_label->setText(tr("结果图同步: "));
    analysis_label->setText(tr("分析图同步: "));

    result_ComboBox = new Cfhs_ComboBox(this);
    analysis_ComboBox = new Cfhs_ComboBox(this);
    button_yes->setText(tr("确定"));
    button_no->setText(tr("取消"));

    layout_comboBox_Up->addWidget(result_label,0,Qt::AlignRight);
    layout_comboBox_Up->addWidget(result_ComboBox);
    layout_comboBox_Down->addWidget(analysis_label,0,Qt::AlignRight);
    layout_comboBox_Down->addWidget(analysis_ComboBox);
    layout_button->addWidget(station_open_button,0,Qt::AlignLeft);
    layout_button->addWidget(button_no,0,Qt::AlignCenter);
    layout_button->addWidget(button_yes,0,Qt::AlignCenter);
    layout_all_comboBox->addLayout(layout_comboBox_Up);
    layout_all_comboBox->addLayout(layout_comboBox_Down);
    layout_comboBox_swbutton->addWidget(result_open_button,0,Qt::AlignLeft);
    layout_comboBox_swbutton->addLayout(layout_all_comboBox);
    layout_all->addWidget(button_result);
    layout_all->addWidget(button_analysis);
    layout_all->addLayout(layout_comboBox_swbutton);

    for (int i=0;i<number;i++)
    {
        int stationNo = i + 1;
        QString str = QString(tr("工位%1")).arg(stationNo);
        Add_Station(str);
        str = QString(tr("工位%1图像路径")).arg(stationNo);
        lineedit_button *button_station = new lineedit_button(str,this);
        layout_all->addWidget(button_station);
        map_station.insert(stationNo, button_station);
    }
    layout_all->addLayout(layout_button);
    layout_all->setContentsMargins(9,9,9,20);
    setLayout(layout_all);

    connect(button_yes,SIGNAL(clicked()),this,SLOT(button_yes_click()));
    connect(button_no,SIGNAL(clicked()),this,SLOT(button_no_click()));
    connect(result_open_button, &switch_button::sig_button_clicked,
            this, &Img_Show_Managent::result_open_button_clicked);
    connect(station_open_button, &switch_button::sig_button_clicked,
            this, &Img_Show_Managent::station_open_button_clicked);
}

Img_Show_Managent::~Img_Show_Managent()
{

}

void Img_Show_Managent::DialogShow()
{
    //获取方案
    stProgramme stPro;
    QString strInfo;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //设置结果路径
    button_result->Set_Path(stPro.strResultImgPath);
    //分析路径
    button_analysis->Set_Path(stPro.strAnalysisImgPath);
    //结果图来源
    result_open_button->Set_Switch(stPro.bSyn);
    //工位图来源
    station_open_button->Set_Switch(stPro.bStationDynamic);
    //工位数
    int stationNum = result_ComboBox->count();
    //结果同步工位号
    if(stPro.iSynResultStationNo <= stationNum)
        result_ComboBox->setCurrentIndex(stPro.iSynResultStationNo-1);
    //分析同步工位号
    if(stPro.iSynAnalysisStationNo <= stationNum)
        analysis_ComboBox->setCurrentIndex(stPro.iSynAnalysisStationNo-1);
    //工位路径
    for(int i=0; i<stationNum; i++)
    {
        //工位号
        int stationNo = i+1;
        stStation station;
        if(!m_logicInterface->GetStationInfo(m_curProgramName, stationNo, station, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        map_station[stationNo]->Set_Path(station.strImgPath);
    }
    this->exec();
}

void Img_Show_Managent::Add_Station(QString station)
{
    result_ComboBox->addItem(station);
    analysis_ComboBox->addItem(station);
}

void Img_Show_Managent::button_yes_click()
{
    //检查结果图路径是否有效
    QString resultPath = button_result->Get_Path();
    if(resultPath.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请设置结果图路径"));
        return;
    }
    //检查分析图路径是否有效
    QString analysisPath = button_analysis->Get_Path();
    if(analysisPath.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请设置分析图路径"));
        return;
    }
    QString strInfo; //日志
    //设置方案信息
    int resultSysNo = result_ComboBox->currentIndex()+1;
    int analysisSysNo = analysis_ComboBox->currentIndex()+1;
    bool isResultSys = result_open_button->Get_Switch();
    bool isStationSys = station_open_button->Get_Switch();
    stProgramme stPro;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    stPro.strResultImgPath = resultPath;
    stPro.strAnalysisImgPath = analysisPath;
    stPro.iSynResultStationNo = resultSysNo;
    stPro.iSynAnalysisStationNo = analysisSysNo;
    stPro.bSyn = isResultSys;
    stPro.bStationDynamic = isStationSys;
    if(!m_logicInterface->SetProInfo(m_curProgramName, stPro, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //检查工位图图路径是否有效
    QMap<int, lineedit_button*>::const_iterator iter = map_station.begin();
    while(iter != map_station.end())
    {
        QString stationPath = iter.value()->Get_Path();
        if(stationPath.isEmpty())
        {
            strInfo = QString(tr("请设置工位%1的图像路径")).arg(iter.key());
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        //设置工位图路径
        stStation station;
        if(!m_logicInterface->GetStationInfo(m_curProgramName, iter.key(), station, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        station.strImgPath = stationPath;
        if(!m_logicInterface->SetStationInfo(m_curProgramName, iter.key(), station, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }

        iter++;
    }
    QMessageBox::information(this, " ", tr("设置成功"));
    this->accept();
}

void Img_Show_Managent::button_no_click()
{
    this->reject();
}

void Img_Show_Managent::result_open_button_clicked()
{
    bool isOpen = result_open_button->Get_Switch();
    isOpen = !isOpen;
    result_open_button->Set_Switch(isOpen);
}

void Img_Show_Managent::station_open_button_clicked()
{
    bool isOpen = station_open_button->Get_Switch();
    isOpen = !isOpen;
    station_open_button->Set_Switch(isOpen);
    //管理结果图来源，工位图为静态时，结果图固定为静态
    result_open_button->Set_Switch_Enable(isOpen);
}
