#include "cfhs_programconfig.h"
#include "ui_cfhs_programconfig.h"
#include "cfhs_defectjudgewidget.h"
#include "cfhs_global.h"
#include "cfhs_messageinput.h"
#include "cfhs_messageselect.h"
#include "cfhs_filterngdialog.h"
#include <QTreeWidgetItem>
#include <QList>
#include <QMessageBox>
#include <QStatusBar>
#include <QFile>

class Cfhs_ShowStatusBar : public QStatusBar
{
public:
    Cfhs_ShowStatusBar(QWidget *parent = nullptr)
        : QStatusBar(parent)
    {
        m_label = new QLabel(this);
        m_label->setAlignment(Qt::AlignCenter);
        this->addWidget(m_label, 1);
    }
    ~Cfhs_ShowStatusBar(){}
    void showMessageInfo(const bool &isNormal, const QString &strMsg)
    {
        QString style = "";
        if(!isNormal)
            style = "QLabel{color:#D42E2E; font-size:16px; font-family:Mircosoft Yahei;}";
        else
            style = "QLabel{color:#2193AB; font-size:16px; font-family:Mircosoft Yahei;}";
        m_label->setStyleSheet(style);
        m_label->setText(strMsg);
    }

private:
    QLabel *m_label;
};


Cfhs_ProgramConfig::Cfhs_ProgramConfig(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cfhs_ProgramConfig)
{
    ui->setupUi(this);
    this->init();
    this->setWindowStyle();
}

Cfhs_ProgramConfig::~Cfhs_ProgramConfig()
{
    delete ui;
}

bool Cfhs_ProgramConfig::ReadProgram(const QString &programName)
{
    if(programName.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请选择要读取的方案名"));
        return false;
    }
    QString strInfo;
    //读取方案
    if(!m_logicInterface->GetProInfo(programName, m_curProgram, false, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    //更新当前方案名
    setCurProgramName(programName);
    //读取工位信息
    QString strAllStation;
    if(!m_logicInterface->GetAllStationNo(programName, strAllStation, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    QStringList list = getListFromQString(strAllStation);
    //清理工位列表
    m_stationList.clear();
    if(!list.isEmpty())
    {
        foreach(QString strStation, list)
        {
            int stationNo = strStation.toInt();
            stStation station;
            if(!m_logicInterface->GetStationInfo(programName, stationNo, station, strInfo))
            {
                QMessageBox::warning(this, " ", strInfo);
                return false;
            }
            //qDebug()<<"Read station" << station.strToolPara;
            //添加到list中
            m_stationList.append(station);
        }
    }
    //默认显示工位一图
    QString strImg = QString("D:/%1/static_img/station%2.jpg").arg(m_strProgramName).arg(1);
    m_imageWindow->setImage(strImg);
    //产品特征表
    stFeatures stFeat;
    if(!m_logicInterface->GetFeaturesInfo(stFeat, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    QString strFeature;
    QStringList realFeatureList;
    if(m_currentLang == English)
        strFeature = stFeat.strEN;
    else if(m_currentLang == SimplifiedChinese)
        strFeature = stFeat.strCH;
    else
        strFeature = stFeat.strTr;
    m_featureList = getListFromQString(strFeature);
    //更新到流程栏中
    m_processBar->setStationList(m_stationList);
    //方案置为已保存状态
    m_isProgramSaved = true;
    return true;
}

void Cfhs_ProgramConfig::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    saveCurrentProgram();
}

void Cfhs_ProgramConfig::init()
{
    //工位设置
    m_stationSetWidget = new Cfhs_StationSet(this);
    //Roi绘图
    m_imageWindow = new Cfhs_ImageWindow(this);
    ui->windowFrame->setWidget(m_imageWindow);
    //工具箱
    m_toolTree = new Cfhs_ProgramToolTree(this);
    ui->toolFrame->setWidget(m_toolTree);
    //流程栏
    m_processBar = new Cfhs_ProgramProcessBar(this);
    connect(m_imageWindow, &Cfhs_ImageWindow::sig_sendRoiPoint,
            m_processBar, &Cfhs_ProgramProcessBar::slot_getRoiPoint);
    connect(m_processBar, &QTabWidget::tabBarClicked,
            this, &Cfhs_ProgramConfig::slot_currentStation_changed);
    ui->processFrame->setWidget(m_processBar);
    //多工位投影标定
    m_stationsStandardize = nullptr;
    //默认不启用多工位判定逻辑
    ui->stationsJudgeCheckBox->setChecked(false);
    ui->stationJudgePushButton->setEnabled(false);
    //默认不启用NG筛选
    ui->ngFilterCheckBox->setChecked(false);
    ui->ngFilterPushButton->setEnabled(false);
    //日志
    m_statusBar = nullptr;
    setCurProgramName("");
}

void Cfhs_ProgramConfig::setWindowStyle()
{
    this->setStyleSheet("QMainWindow{background:#000A19; border:2px solid #0077FF; font-size:16px;"
                        "font-family:Mircosoft Yahei; color:#0077FF}");
    //图片
    ui->windowFrame->setStyleSheet("QFrame#windowFrame{border:1px solid #0077FF}");
    //工具箱
    ui->toolFrame->setStyleSheet("QFrame#toolFrame{border:1px solid #0077FF}");
    //流程栏
    ui->processFrame->setStyleSheet("QFrame#processFrame{border:1px solid #0077FF}");
}

bool Cfhs_ProgramConfig::isProgramNameValid()
{
    if(m_strProgramName.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请新建或者读取方案"));
        return false;
    }

    return true;
}

bool Cfhs_ProgramConfig::isStationListValid()
{
    if(m_stationList.isEmpty())
    {
        QString info = QString(tr("方案(%1)没有工位信息，请添加")).arg(m_strProgramName);
        QMessageBox::warning(this, " ", info);
        return false;
    }

    return true;
}

bool Cfhs_ProgramConfig::isMultipleStationValid()
{
    int size = m_stationList.size();
    if(size < 2)
    {
        QString strInfo = QString(tr("方案(%1)不是多工位，不支持该功能")).arg(m_strProgramName);
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }

    return true;
}

void Cfhs_ProgramConfig::setCurProgramName(const QString &name)
{
    m_strProgramName = name;
    //设置日志
    if(!m_statusBar)
    {
        m_statusBar = new Cfhs_ShowStatusBar(this);
        this->setStatusBar(m_statusBar);
    }
    QString strMsg;
    bool isNormal = false;
    if(m_strProgramName.isEmpty())
    {
        strMsg = QString(tr("当前方案为空"));
        isNormal = false;
    }
    else
    {
        strMsg = QString(tr("当前方案为：%1")).arg(m_strProgramName);
        isNormal = true;
    }
    m_statusBar->showMessageInfo(isNormal, strMsg);
}

void Cfhs_ProgramConfig::saveCurrentProgram()
{
    if(!m_strProgramName.isEmpty()
            && !m_isProgramSaved)
    {
        QString strInfo = QString(tr("当前方案(%1)未保存，是否保存？")).arg(m_strProgramName);
        QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                           tr("提示"),
                                           strInfo,
                                           QMessageBox::Yes|
                                           QMessageBox::No,
                                           this);
        msg->setButtonText(QMessageBox::Yes, tr("是"));
        msg->setButtonText(QMessageBox::No, tr("否"));
        msg->setDefaultButton(QMessageBox::Yes);
        if(msg->exec() == QMessageBox::Yes)
            on_saveProgramAction_triggered();

        delete msg;
        msg = nullptr;
    }
}

void Cfhs_ProgramConfig::slot_currentStation_changed(const int &index)
{
    if(index <0 || index>= m_stationList.size())
        return;
    if(m_strProgramName.isEmpty())
        return;
    int stationNo = index + 1;
    QString strImg = QString("D:/%1/static_img/station%2.jpg").arg(m_strProgramName).arg(stationNo);
    if(!QFile::exists(strImg))
    {
        m_imageWindow->setImage(strImg);
        return;
    }
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                       tr("提示"),
                                       tr("工位图加载中，请稍后..."),
                                       QMessageBox::Ok,
                                       this);
    msg->show();
    Sleep(10);
    m_imageWindow->setImage(strImg);
    msg->close();
    delete msg;
    msg = nullptr;
}

void Cfhs_ProgramConfig::on_newProgramAction_triggered()
{
    //保存当前方案
    saveCurrentProgram();
    //新建方案名
    QString name = tr("新建方案");
    QString strInfo;
    //已有方案
    QString strAllPro;
    if(!m_logicInterface->GetAllProName(strAllPro, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QStringList listPro = getListFromQString(strAllPro);
    Cfhs_MessageInput message(name, this, listPro);
    if(message.exec() == QDialog::Accepted)
    {
        QString newProgramName = message.getMessage();
        //插入方案
        if(!m_logicInterface->InsertNewProName(newProgramName, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        //读取新的方案
        if(ReadProgram(newProgramName))
        {
            strInfo = QString(tr("方案(%1)新建成功")).arg(newProgramName);
            QMessageBox::information(this, " ", strInfo);
        }
    }
}

void Cfhs_ProgramConfig::on_readProgramAction_triggered()
{
    //保存当前方案
    saveCurrentProgram();
    //获取方案列表
    QString strInfo; //日志信息
    QString programInfo;
    if(!m_logicInterface->GetAllProName(programInfo, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QString name = tr("方案名");
    QStringList list = getListFromQString(programInfo);  //方案列表
    Cfhs_MessageSelect msg(name, list, this);
    if(msg.exec() == QDialog::Accepted)
    {
        //读取方案
        QString proName = msg.getMessage();
        QString strMsg = QString(tr("方案(%1)读取中，请稍后...")).arg(proName);
        QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                           tr("提示"),
                                           strMsg,
                                           QMessageBox::Ok,
                                           this);
        msg->show();
        Sleep(10);
        if(ReadProgram(proName))
        {
            msg->close();
            delete msg;
            msg = nullptr;
            QString strMsg = QString(tr("方案(%1)读取成功")).arg(proName);
            QMessageBox::information(this, " ", strMsg);
        }
    }
}

void Cfhs_ProgramConfig::on_saveProgramAction_triggered()
{
    if(!isProgramNameValid())
        return;
    QString strInfo;
    //保存方案
    if(!m_logicInterface->SetProInfo(m_strProgramName, m_curProgram, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //更新工位流程栏信息
    QMap<int, QString> processMap = m_processBar->getProgramProcessMap();
    if(processMap.size() != m_stationList.size())
    {
        QMessageBox::warning(this, " ", tr("获取的流程栏信息错误，与工位个数不匹配"));
        return;
    }
    for(int i=0; i<m_stationList.size(); i++)
    {
        int stationNo = m_stationList[i].iStationNo;
        m_stationList[i].strToolPara = processMap.value(stationNo);
    }
    //保存工位信息
    foreach(stStation station, m_stationList)
    {
        if(!m_logicInterface->SetStationInfo(m_strProgramName, station.iStationNo, station, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        //qDebug()<<"Save station"<<station.strToolPara;
    }
    strInfo = QString(tr("方案(%1)保存成功")).arg(m_strProgramName);
    m_isProgramSaved = true;
    QMessageBox::information(this, " ", strInfo);
}

void Cfhs_ProgramConfig::on_deleteProgramAction_triggered()
{
    //获取方案列表
    QString strInfo; //日志信息
    QString programInfo;
    if(!m_logicInterface->GetAllProName(programInfo, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QString name = tr("方案名");
    QStringList list = getListFromQString(programInfo); //方案列表
    Cfhs_MessageSelect msg(name, list, this);
    if(msg.exec() == QDialog::Accepted)
    {
        //读取方案
        QString proName = msg.getMessage();
        //删除方案
        if(!m_logicInterface->DeleteProName(proName, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        //如果删除的是当前方案，当前方案清空
        if(proName == m_strProgramName)
        {
            //清空流程栏
            foreach(stStation station, m_stationList)
            {
                m_processBar->delStation(station.iStationNo);
            }
            m_processBar->clear();
            //清空工位列表
            m_stationList.clear();
            //方案名重置
            setCurProgramName("");
        }
        strInfo = QString(tr("方案(%1)删除成功")).arg(proName);
        QMessageBox::information(this, " ", strInfo);
    }
}

void Cfhs_ProgramConfig::on_stationAddPushButton_clicked()
{
    if(!isProgramNameValid())
        return;
    QString strInfo;
    //添加工位
    int addStationNo = m_stationList.size()+1;
    if(!m_logicInterface->InsertNewStationNo(m_strProgramName, addStationNo, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //回读工位信息
    stStation station;
    if(!m_logicInterface->GetStationInfo(m_strProgramName, addStationNo, station, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //添加到list中
    m_stationList.append(station);
    //添加到流程栏中
    m_processBar->addStation(station);
    //方案已改变，修改状态
    m_isProgramSaved = false;
}

void Cfhs_ProgramConfig::on_stationDelPushButton_clicked()
{
    if(!isProgramNameValid())
        return;
    if(!isStationListValid())
        return;
    int stationNo = m_stationList.size();
    QString strInfo = QString(tr("确定删除工位%1")).arg(stationNo);
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                       tr("删除"),
                                       strInfo,
                                       QMessageBox::Yes|
                                       QMessageBox::No,
                                       this);
    msg->setButtonText(QMessageBox::Yes, tr("是"));
    msg->setButtonText(QMessageBox::No, tr("否"));
    msg->setDefaultButton(QMessageBox::Yes);
    if(msg->exec() == QMessageBox::Yes)
    {
        //删除工位
        if(!m_logicInterface->DeleteStationNo(m_strProgramName, stationNo, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        //从流程栏中删除
        m_processBar->delStation(stationNo);
        //从列表中删除
        m_stationList.pop_back();
        //方案已改变，修改状态
        m_isProgramSaved = false;
    }
    delete msg;
    msg = nullptr;
}

void Cfhs_ProgramConfig::on_stationSetPushButton_clicked()
{
    if(!m_stationSetWidget)
    {
        m_stationSetWidget = new Cfhs_StationSet(this);
    }
    if(!isProgramNameValid())
        return;
    if(!isStationListValid())
        return;
    //工位信息
    StationInfoList stationList;
    QString strInfo;
    foreach(stStation station, m_stationList)
    {
        StationInfo info;
        info.m_stationNo = station.iStationNo;
        info.m_ip = station.strIP;
        info.m_port = station.usPort;
        info.m_defectNum = station.iDefectNum;
        info.m_isEnable = station.bEnable;
        if(!getMapFromJson(station.strDefectDefine, info.m_mapDefect, strInfo))
        {
            strInfo = QString(tr("工位%1的缺陷定义参数错误：%2")).arg(station.iStationNo).arg(strInfo);
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        if(!getMapFromJson(station.strDefectScreening, info.m_mapNg, strInfo))
        {
            strInfo = QString(tr("工位%1的NG判定参数错误：%2")).arg(station.iStationNo).arg(strInfo);
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        stationList.append(info);
    }
    m_stationSetWidget->setStationList(stationList);
    m_stationSetWidget->exec();
    //更新工位信息
    stationList = m_stationSetWidget->getStationList();
    if(stationList.size() != m_stationList.size())
    {
        strInfo = QString(tr("生成的工位参数有误，与该方案的工位数量不匹配"));
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //为了确保数据的准确性，匹配工位号
    foreach(StationInfo info, stationList)
    {
        for(int i=0; i<stationList.size();i++)
        {
            if(m_stationList[i].iStationNo == info.m_stationNo)
            {
                m_stationList[i].strIP = info.m_ip;
                m_stationList[i].usPort = static_cast<quint16>(info.m_port);
                m_stationList[i].iDefectNum = info.m_defectNum;
                m_stationList[i].bEnable = info.m_isEnable;
                m_stationList[i].strDefectDefine = getJsonFromMap(info.m_mapDefect);
                m_stationList[i].strDefectScreening = getJsonFromMap(info.m_mapNg);
                break;
            }
        }
    }
    //方案已改变
    m_isProgramSaved = false;
}

void Cfhs_ProgramConfig::on_resloutionSetPushButton_clicked()
{

}

void Cfhs_ProgramConfig::on_stationJudgePushButton_clicked()
{
    if(!m_stationsStandardize)
    {
        m_stationsStandardize = new Cfhs_StationsVirtualStandardize(this);
    }
    if(!isProgramNameValid())
        return;
    if(!isMultipleStationValid())
        return;
    //获取工位列表
    QString strAllStation;
    QString strInfo;
    if(!m_logicInterface->GetAllStationNo(m_strProgramName, strAllStation, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QStringList stationList = getListFromQString(strAllStation);
    if(!m_stationsStandardize->setStationStandardize(stationList, m_curProgram.strDefectDetermination))
        return;
    if(m_stationsStandardize->exec() == QDialog::Accepted)
    {
        m_curProgram.strDefectDetermination = m_stationsStandardize->getStationStandardizeInfo();
        m_curProgram.strNGScreening = m_stationsStandardize->getNgFilterInfo();
        QMessageBox::information(this, " ", tr("设置成功"));
        m_isProgramSaved = false;
    }
}

void Cfhs_ProgramConfig::on_ngFilterPushButton_clicked()
{
    if(!isProgramNameValid())
        return;
    if(!isMultipleStationValid())
        return;
    Cfhs_FilterNgDialog dialog(m_featureList, this);
    if(!dialog.setNgFilterInfo(m_curProgram.strNGScreening))
        return;
    if(dialog.exec() == QDialog::Accepted)
    {
        m_curProgram.strNGScreening = dialog.getNgFilterInfo();
        QMessageBox::information(this, " ", tr("NG筛选参数设置成功"));
        //方案已改变，修改状态
        m_isProgramSaved = false;
    }
}

void Cfhs_ProgramConfig::on_stationsJudgeCheckBox_toggled(bool checked)
{
    ui->stationJudgePushButton->setEnabled(checked);
}

void Cfhs_ProgramConfig::on_ngFilterCheckBox_toggled(bool checked)
{
    ui->ngFilterPushButton->setEnabled(checked);
}

