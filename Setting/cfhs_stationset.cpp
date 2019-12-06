#include "cfhs_stationset.h"
#include "ui_cfhs_stationset.h"
#include "cfhs_base.h"
#include "cfhs_global.h"
#include <QPixmap>
#include <QIcon>
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>


Cfhs_StationSet::Cfhs_StationSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cfhs_StationSet)
{
    ui->setupUi(this);
    this->init();
}

Cfhs_StationSet::~Cfhs_StationSet()
{
    delete ui;
}

StationInfoList Cfhs_StationSet::getStationList() const
{
    return m_stationList;
}

void Cfhs_StationSet::setStationList(const StationInfoList& list)
{
    m_stationList = list;
    //将工位名添加到listWidget
    QStringList nameList;
    for(int i=0; i<m_stationList.size(); i++)
    {
        StationInfo station = m_stationList.at(i);
        //将工位名添加到namelist中
        QString name = QString(tr("工位%1")).arg(station.m_stationNo);
        nameList.append(name);
    }
    //设置stationListWidget
    ui->stationListWidget->clear();
    ui->stationListWidget->addItems(nameList);
    ui->stationListWidget->setCurrentRow(0);
    int stationNo = ui->stationListWidget->currentRow()+1;
    //找到当前行对应的工位
    foreach(StationInfo station, m_stationList)
    {
        if(stationNo == station.m_stationNo)
        {
            m_curStation = station;
            break;
        }
    }
    showStationData();
}

void Cfhs_StationSet::setFeatureList(const QStringList &list)
{
    QStringList listDefect = list;
    //缺陷定义去掉距离和个数
    if(m_currentLang == SimplifiedChinese)
    {
        listDefect.removeOne("距离");
        listDefect.removeOne("个数");
    }
    else if(m_currentLang == TraditionalChinese)
    {
        listDefect.removeOne("距離");
        listDefect.removeOne("個數");
    }
    else
    {
        listDefect.removeOne("DefectDis");
        listDefect.removeOne("DefectNum");
    }
    m_defectInfoWidget->setFeatherList(listDefect);
    //NG筛选
    m_ngFilterWidget->setFeatureList(list);
}

void Cfhs_StationSet::showStationData()
{
    //设置IP地址
    ui->stationIpLineEdit->setText(m_curStation.m_ip);
    //设置IP端口
    ui->stationPortSpinBox->setValue(m_curStation.m_port);
    //设置工位是否启用
    setStationEnable(m_curStation.m_isEnable);
    //设置缺陷数量
    ui->defectNumSpinBox->setValue(m_curStation.m_defectNum);
    //设置缺陷定义
    m_defectInfoWidget->setDefectInfoMap(m_curStation.m_mapDefect);
    //设置NG判定
    m_ngFilterWidget->setInfoMap(m_curStation.m_mapNg);
    //设置缺陷特征
    setFeatureList(m_curStation.m_listFeature);
}

void Cfhs_StationSet::closeEvent(QCloseEvent *event)
{
    //更新数据
    updateData();
    //检查Ip是否合法
    foreach(StationInfo station, m_stationList)
    {
        m_curStation = station;
        if(!isIpValid(station.m_ip))
        {
            event->ignore();
            return;
        }
        if(isIpRepeated(station.m_ip))
        {
            event->ignore();
            return;
        }
    }
}

void Cfhs_StationSet::init()
{
    this->setWindowFlags(this->windowFlags()|Qt::WindowMaximizeButtonHint);
    m_stationList.clear();
    m_curStation.init();
    //工位端口设置
    const int portMin = 1, portMax = 65535;
    ui->stationPortSpinBox->setRange(portMin, portMax);
    //工位缺陷数量设置
    const int numMin = 0, numMax = 1000;
    ui->defectNumSpinBox->setRange(numMin, numMax);
    //设置缺陷信息窗口
    m_defectInfoWidget = new Cfhs_DefectInfoWidget(this);
    ui->defectFrame->setWidget(m_defectInfoWidget);
    //NG判定
    m_ngFilterWidget = new Cfhs_DefectInfoTable(tr("工位NG判定"), QStringList(), false, this);
    m_ngFilterWidget->setHeadLabel(2, tr("NG判定方式"));
    ui->ngFrame->setWidget(m_ngFilterWidget);
    //缺陷定义窗口对缺陷名称的操作关联到NG判定窗口
    connect(m_defectInfoWidget, &Cfhs_DefectInfoWidget::sig_addOneRow,
            m_ngFilterWidget, &Cfhs_DefectInfoTable::addOneRow);
    connect(m_defectInfoWidget, &Cfhs_DefectInfoWidget::sig_editOneRow,
            m_ngFilterWidget, &Cfhs_DefectInfoTable::editOneRow);
    connect(m_defectInfoWidget, &Cfhs_DefectInfoWidget::sig_deleteOneRow,
            m_ngFilterWidget, &Cfhs_DefectInfoTable::delOneRow);
}

void Cfhs_StationSet::setStationEnable(const bool& isEnable)
{
    QString style ="";
    if(isEnable)
    {
        style = QString("QPushButton{border-image:url(:/button_check.png);"
                        "border:none;background:transparent;width:75px;height:34px;}");
    }
    else
        style = QString("QPushButton{border-image:url(:/button_uncheck.png);"
                        "border:none;background:transparent;width:75px;height:34px;}");
    ui->stationEnablePushButton->setStyleSheet(style);
    m_curStation.m_isEnable = isEnable;
}

//刷新数据
void Cfhs_StationSet::updateData()
{
    //获取缺陷定义参数
    m_curStation.m_mapDefect = m_defectInfoWidget->getDefectInfoMap();
    //获取Ng判定参数
    m_ngFilterWidget->getInfoMap(m_curStation.m_mapNg);
    int stationNo = m_curStation.m_stationNo;
    for(int i=0; i<m_stationList.size(); i++)
    {
        StationInfo station = m_stationList.at(i);
        if(station.m_stationNo == stationNo)
        {
            m_stationList[i] = m_curStation;
            return;
        }
    }
}

bool Cfhs_StationSet::isIpValid(const QString &ip)
{
     QRegExp rx2("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    if(!rx2.exactMatch(ip))
    {
        QMessageBox::warning(this, " ", tr("ip地址错误"));
        return false;
    }
    return true;
}

void Cfhs_StationSet::on_stationIpLineEdit_textEdited(const QString &curText)
{
    m_curStation.m_ip = curText;
}

void Cfhs_StationSet::on_stationEnablePushButton_clicked()
{
    bool isEnable = !m_curStation.m_isEnable;
    //设置button状态
    setStationEnable(isEnable);
}

void Cfhs_StationSet::on_stationPortSpinBox_valueChanged(int value)
{
    m_curStation.m_port = value;
}

void Cfhs_StationSet::on_defectNumSpinBox_valueChanged(int value)
{
    m_curStation.m_defectNum = value;
}

void Cfhs_StationSet::on_stationListWidget_currentRowChanged(int currentRow)
{
    if(ui->stationListWidget->hasFocus())
    {
        //将当前界面上的值更新到列表中
        updateData();
        //刷新界面
        //找到当前行对应的工位
        int stationNo = currentRow + 1;
        foreach(StationInfo station, m_stationList)
        {
            if(stationNo == station.m_stationNo)
            {
                m_curStation = station;
                break;
            }
        }
        showStationData();
    }
}

bool Cfhs_StationSet::isIpRepeated(const QString &ip)
{
    foreach(StationInfo station, m_stationList)
    {
        //本工位忽略
        if(station.m_stationNo == m_curStation.m_stationNo)
            continue;
        else
        {
            if(station.m_ip == ip)
            {
                QString errorStr = QString(tr("工位%1与工位%2的IP相同，请重新设置"))
                        .arg(station.m_stationNo)
                        .arg(m_curStation.m_stationNo);
                QMessageBox::warning(this, " ", errorStr);
                return true;
            }
        }
    }

    return false;
}

bool Cfhs_StationSet::isDefectInfoValid(const QMap<QString, QString> &map)
{
    if(!map.isEmpty())
    {
        QMap<QString, QString>::const_iterator iter = map.begin();
        while(iter != map.end())
        {
            QString strKey = iter.key();
            if(strKey.isEmpty())
            {
                QMessageBox::warning(this, tr("错误"), tr("缺陷名称不能为空"));
                return false;
            }
            QString strVal = iter.value();
            if(strVal.isEmpty())
            {
                QString errorStr = QString(tr("%1的判定条件不能为空")).arg(strKey);
                QMessageBox::warning(this, tr("错误"), errorStr);
                return false;
            }

            iter++;
        }
    }

    return true;
}
