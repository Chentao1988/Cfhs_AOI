#include "ui_cfhs_mainwindow.h"
#include "cfhs_mainwindow.h"
#include "cfhs_messageselect.h"
#include "Function/img_show_managent.h"
#include "Function/imgsavemode.h"
#include "Help/cfhs_softwareversion.h"
#include "cfhs_addcamerainfo.h"
#include "Help/cfhs_defectcallout.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QToolButton>
#include <QtMath>
#include <QMessageBox>
#include <QDateTime>
#include <QHeaderView>
#include <QDesktopServices>
#include <QUrl>


Cfhs_MainWindow::Cfhs_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cfhs_MainWindow)
{
    this->mainWindowInit();
    this->setWindowStyle();
    //信号槽连接
    //弹窗信息
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowFrameInfo,
            this, &Cfhs_MainWindow::slot_ShowFrameInfo);
    //---显示日志信息
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowInfo,
            this, &Cfhs_MainWindow::slot_ShowInfo);
    //---显示扫码率
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowScanRate,
            this, &Cfhs_MainWindow::slot_ShowScanRate);
    //---显示Mes上报率
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowMesRate,
            this, &Cfhs_MainWindow::slot_ShowMesRate);
    //---显示特征信息
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowFeatureData,
            this, &Cfhs_MainWindow::slot_ShowFeatureData);
    //---显示工位连接状态
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowStationStatus,
            this, &Cfhs_MainWindow::sig_ShowStationStatus);
    //---显示当前产品结果
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowNowResult,
            this, &Cfhs_MainWindow::sig_ShowNowResult);
    //---显示前20个的结果
    connect(m_logicInterface, &Cfhs_IBusiness::sig_Show20Result,
            this, &Cfhs_MainWindow::sig_Show20Result);
    //---显示批次表单信息
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowBatchFormInfo,
            this, &Cfhs_MainWindow::slot_ShowBatchFormInfo);
    //---显示工位图片和缺陷点
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowStationImagePoint,
            this, &Cfhs_MainWindow::slot_ShowStationImagePoint);
    //---显示结果图片和缺陷点
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowResultImagePoint,
            this, &Cfhs_MainWindow::slot_ShowResultImagePoint);
    //---显示分析图片和缺陷点
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowAnalysisImagePoint,
            this, &Cfhs_MainWindow::slot_ShowAnalysisImagePoint);
    //---显示批次图表信息
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowBatchChartInfo,
            this, &Cfhs_MainWindow::slot_ShowBatchChartInfo);
    //---磁盘监控报警
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowDiskSpaceAlarm,
            this, &Cfhs_MainWindow::slot_ShowDiskSpaceAlarm);
    //---良率报警
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowYieldAlarm,
            this, &Cfhs_MainWindow::slot_ShowYieldAlarm);
    //系统运行状态
    connect(m_logicInterface, &Cfhs_IBusiness::sig_ShowSoftStatus,
            this, &Cfhs_MainWindow::slot_ShowSystemRunStatus);

    //读取方案
    ReadProgram();
    //虚拟数据
    //createData();
}

Cfhs_MainWindow::~Cfhs_MainWindow()
{
    if(m_logicInterface)
    {
        m_logicInterface->Stop();
        m_logicInterface->Exit();
        delete m_logicInterface;
        m_logicInterface = nullptr;
    }
    delete ui;
}

void Cfhs_MainWindow::controlInit()
{
    //控制项Menu
    m_controlMenu = new QMenu(this);
    //硬盘监控
    m_diskMonitorAction = new QAction(this);
    m_diskMonitorAction->setText(tr("电脑硬盘监控管理"));
    connect(m_diskMonitorAction, &QAction::triggered,
            this, &Cfhs_MainWindow::diskMonitorAction_triggered);
    //软件维护
    m_softMaintainAction = new QAction(this);
    m_softMaintainAction->setText(tr("软件维护功能"));
    connect(m_softMaintainAction, &QAction::triggered,
            this, &Cfhs_MainWindow::softMaintainAction_triggered);
    //良率预警
    m_yieldWarningAction = new QAction(this);
    m_yieldWarningAction->setText(tr("实时良率预警"));
    connect(m_yieldWarningAction, &QAction::triggered,
            this, &Cfhs_MainWindow::yieldWarningAction_triggered);
    //将action添加到Menu上
    m_controlMenu->addAction(m_diskMonitorAction);
    m_controlMenu->addAction(m_softMaintainAction);
    m_controlMenu->addAction(m_yieldWarningAction);
    //将Menu添加到Control button上
    ui->controlPushButton->setMenu(m_controlMenu);
    //添加事件过滤器
    ui->controlLabel->installEventFilter(this);
}

void Cfhs_MainWindow::settingInit()
{
    //方案配置
    m_programConfigAction = new QAction(this);
    m_programConfigAction->setText(tr("方案配置"));
    connect(m_programConfigAction, &QAction::triggered,
            this, &Cfhs_MainWindow::programConfigAction_triggered);
    //图像拼接管理
    m_imageSpliceAction = new QAction(this);
    m_imageSpliceAction->setText(tr("图像拼接管理"));
    connect(m_imageSpliceAction, &QAction::triggered,
            this, &Cfhs_MainWindow::imageSpliceAction_triggered);
    // NG/OK信号管理
    m_signalNgAction = new QAction(this);
    m_signalNgAction->setText(tr("NG/OK信号管理"));
    connect(m_signalNgAction, &QAction::triggered,
            this, &Cfhs_MainWindow::signalNgAction_triggered);
    //添加相机记录
    m_addCameraAction = new QAction(this);
    m_addCameraAction->setText(tr("添加相机记录"));
    connect(m_addCameraAction, &QAction::triggered,
            this, &Cfhs_MainWindow::addCameraAction_triggered);
    //Menu 设置
    m_settingMenu = new QMenu(this);
    //Action添加到Menu中
    m_settingMenu->addAction(m_programConfigAction);
    m_settingMenu->addAction(m_imageSpliceAction);
    m_settingMenu->addAction(m_signalNgAction);
    m_settingMenu->addAction(m_addCameraAction);
    //将menu添加到设置Button上
    ui->settingPushButton->setMenu(m_settingMenu);
    //方案配置窗口
    m_programConfigWidget = nullptr;
    //添加事件过滤器
    ui->settingLabel->installEventFilter(this);
    //默认隐藏“添加相机记录”功能
    showAdditionalFunction(false);
}


void Cfhs_MainWindow::setWindowStyle()
{
    //主窗口背景色
    this->setStyleSheet("Cfhs_MainWindow{border-image: url(:/main_background.png)}");
    //左右窗口背景色
    ui->leftFrame->setStyleSheet("QFrame#leftFrame{border-image: url(:/main_left_show.png)}");
    ui->rightFrame->setStyleSheet("QFrame#rightFrame{border-image: url(:/main_left_show.png)}");
    //大图背景色
    ui->mainFrame->setStyleSheet("QFrame#mainFrame{border-image: url(:/big_image_back.png)}");
    //附图背景
    ui->zoomFrame->setStyleSheet("QFrame#zoomFrame{border-image: url(:/mid_image_back.png)}");
    //小图背景
    ui->sub1Frame->setStyleSheet("QFrame#sub1Frame{border-image: url(:/small_image_back.png)}");
    ui->sub2Frame->setStyleSheet("QFrame#sub2Frame{border-image: url(:/small_image_back.png)}");
    ui->sub3Frame->setStyleSheet("QFrame#sub3Frame{border-image: url(:/small_image_back.png)}");
    //账号
    QSize size = QSize(50, 50);
    QString labelStyle = QString("QLabel{color:#0077FF; font-size: 26px;}"
                                 "QLabel:hover{color:#2193AB}");
    ui->userImageLabel->setStyleSheet("QLabel{border-image: url(:/user_normal.png);}"
                                      "QLabel:hover{border-image: url(:/user_press.png)}");
    ui->userImageLabel->setFixedSize(size);
    ui->userImageLabel->setText("");
    ui->userTextLabel->setStyleSheet(labelStyle);
    //方案
    ui->programPushButton->setFixedSize(size);
    ui->programPushButton->setText("");
    QString programStyle = getFunctionButtonStyle("program");
    ui->programPushButton->setStyleSheet(programStyle);
    ui->programPushButton->setEnabled(true);
    ui->programTextLabel->setStyleSheet(labelStyle);
    ui->programTextLabel->setText("方案名");
    //系统状态,默认为未启动状态
    setSystemStatus(false);
    //启动
    size = QSize(64, 64);
    ui->startPushButton->setFixedSize(size);
    ui->startPushButton->setText("");
    QString startStyle = getFunctionButtonStyle("start");
    ui->startPushButton->setStyleSheet(startStyle);
    //暂停
    ui->stopPushButton->setFixedSize(size);
    ui->stopPushButton->setText("");
    QString stopStyle = getFunctionButtonStyle("stop");
    ui->stopPushButton->setStyleSheet(stopStyle);
    //重连
    ui->connectPushButton->setFixedSize(size);
    ui->connectPushButton->setText("");
    QString connectStyle = getFunctionButtonStyle("connect");
    ui->connectPushButton->setStyleSheet(connectStyle);
    //关闭
    ui->closePushButton->setFixedSize(size);
    ui->closePushButton->setText("");
    QString closeStyle = getFunctionButtonStyle("close");
    ui->closePushButton->setStyleSheet(closeStyle);
    //功能
    size = QSize(34, 34);
    ui->functionPushButton->setFixedSize(size);
    ui->functionPushButton->setText("");
    QString functionStyle = getFunctionButtonStyle("function");
    ui->functionPushButton->setStyleSheet(functionStyle);
    //字体设置为22px
    labelStyle = QString("QLabel{color:#0077FF; font-size: 26px; font-family:Mircosoft Yahei;}");
    ui->functionLabel->setStyleSheet(labelStyle);
    //控制
    ui->controlPushButton->setFixedSize(size);
    ui->controlPushButton->setText("");
    QString controlStyle = getFunctionButtonStyle("control");
    ui->controlPushButton->setStyleSheet(controlStyle);
    ui->controlLabel->setStyleSheet(labelStyle);
    //设置
    ui->settingPushButton->setFixedSize(size);
    ui->settingPushButton->setText("");
    QString setStyle = getFunctionButtonStyle("set");
    ui->settingPushButton->setStyleSheet(setStyle);
    ui->settingLabel->setStyleSheet(labelStyle);
    //帮助
    ui->helpPushButton->setFixedSize(size);
    ui->helpPushButton->setText("");
    QString helpStyle = getFunctionButtonStyle("help");
    ui->helpPushButton->setStyleSheet(helpStyle);
    ui->helpLabel->setStyleSheet(labelStyle);
    //读码开关
    setReadCodingOpen(false);
    labelStyle = QString("QLabel{color:#2193AB; font-size: 18px}");
    ui->readCodeLabel->setStyleSheet(labelStyle);
    //MES上传开关
    setMesReportOpen(true);
    ui->mesReportLabel->setStyleSheet(labelStyle);
    //RadioButton 样式
    ui->resultLabel->setStyleSheet(labelStyle);
    //产品良率实时报表
    ui->batchChartFrame->setStyleSheet("QFrame#batchChartFrame{border-image: url(:/main_left_show.png)}");
    m_batchChart->setWidgetStyle();
    //侧边栏
    ui->sideFrame->setStyleSheet("QFrame#sideFrame{background:#071C50}");
}

void Cfhs_MainWindow::mainWindowInit()
{
    ui->setupUi(this);
    this->setFixedSize(1920, 1080);
    this->setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);
    //工位栏
    m_taskWidget = new Cfhs_TaskInfoWidget(this, true);
    ui->taskFrame->setWidget(m_taskWidget);
    connect(this, &Cfhs_MainWindow::sig_ShowStationStatus,
            m_taskWidget, &Cfhs_TaskInfoWidget::slot_ShowStationStatus);
    connect(m_taskWidget, &Cfhs_TaskInfoWidget::sig_ShowCurrentTask,
            this, &Cfhs_MainWindow::slot_ShowCurrentTask);
    //大图
    m_bigImageWidget = new cfhs_mainwindows_img(this);
    m_bigImageWidget->installEventFilter(this);
    ui->mainFrame->setWidget(m_bigImageWidget);
    m_sumAnalysisTime = 12;
    //同步九宫格参数
    m_bigImageWidget->getGridView(m_openGridview, m_xGridview, m_yGridview);
    //放大图
    m_zoomImageWidget = new Cfhs_ShowZoomImage(this);
    ui->zoomFrame->setWidget(m_zoomImageWidget);
    connect(m_zoomImageWidget, &Cfhs_ShowZoomImage::sig_setMultiple,
            m_bigImageWidget, &cfhs_mainwindows_img::setTimes);
    connect(m_bigImageWidget, &cfhs_mainwindows_img::sig_showImage,
            m_zoomImageWidget, &Cfhs_ShowZoomImage::slot_showImage);
    //三张缺陷小图
    for(int i=0; i<3; i++)
    {
        m_defectSmallImage[i] = new Cfhs_DefectSmallImage(this);
    }
    ui->sub1Frame->setWidget(m_defectSmallImage[0]);
    ui->sub2Frame->setWidget(m_defectSmallImage[1]);
    ui->sub3Frame->setWidget(m_defectSmallImage[2]);
    //显示特征信息表
    m_featureTable = new Cfhs_TableWidget(this);
    m_featureTable->setMouseEventInfo(true, 4);  //接收鼠标左键按下事件
    m_featureTable->setContextEventUsed(true);  //接收Context事件
    ui->featureFormFrame->setWidget(m_featureTable);
    connect(m_featureTable, &Cfhs_TableWidget::sig_showPoint,
            m_bigImageWidget, &cfhs_mainwindows_img::showLightPoint);
    //显示批次表单信息
    m_batchTable = new Cfhs_TableWidget(this);
    ui->batchFormFrame->setWidget(m_batchTable);
    //显示产品结果
    m_resultWidget = new Cfhs_ProductResultWidget(this);
    ui->resultFrame->setWidget(m_resultWidget);
    connect(this, &Cfhs_MainWindow::sig_ShowNowResult,
            m_resultWidget, &Cfhs_ProductResultWidget::slot_ShowNowResult);
    connect(this, &Cfhs_MainWindow::sig_Show20Result,
            m_resultWidget, &Cfhs_ProductResultWidget::slot_Show20Result);
    //统计图表
    m_batchChart = new Cfhs_Barchart();
    ui->batchChartFrame->setWidget(m_batchChart);
    //侧边工具栏设置
    m_sideToolWidget = new Cfhs_SideTool(this);
    ui->sideFrame->setWidget(m_sideToolWidget);
    //设置状态栏，主要显示日志/告警信息
    m_statusBar = new Cfhs_StatusBar(this);
    this->setStatusBar(m_statusBar);
    m_isSystemStatus = false;
    //日志弹出窗口
    m_showMessage = new QMessageBox(QMessageBox::Warning,
                                    tr("日志显示"),
                                    tr("日志显示"),
                                    QMessageBox::Yes,
                                    this);
    m_showMessage->hide();
    //算法Roi
    m_roiTable = new Cfhs_TableWidget(this);
    ui->RoiFrame->setWidget(m_roiTable);
    //关闭按钮添加事件过滤器
    m_closeTimer = new QTimer(this);
    m_closeTimer->setInterval(3000);
    connect(m_closeTimer, &QTimer::timeout,
            this, &Cfhs_MainWindow::slot_closeButton_timeout);
    ui->closePushButton->installEventFilter(this);
    //控制栏配置
    controlInit();
    //设置栏配置
    settingInit();
    //功能栏配置
    functionInit();
    //帮助栏配置
    helpInit();
}

void Cfhs_MainWindow::setFunctionEnable(const bool &isEnable)
{
    //功能键
    ui->functionPushButton->setEnabled(isEnable);
    ui->functionLabel->setEnabled(isEnable);
    //控制键
    ui->controlLabel->setEnabled(isEnable);
    ui->controlPushButton->setEnabled(isEnable);
    //设置键
    ui->settingLabel->setEnabled(isEnable);
    ui->settingPushButton->setEnabled(isEnable);
    //帮助键
    //ui->helpLabel->setEnabled(isEnable);
    //ui->helpPushButton->setEnabled(isEnable);
    //解码开关
    ui->readCodePushButton->setEnabled(isEnable);
    //Mes开关
    ui->mesReportPushButton->setEnabled(isEnable);
    //结果输出
    ui->normalRadioButton->setEnabled(isEnable);
    ui->okRadioButton->setEnabled(isEnable);
    ui->ngRadioButton->setEnabled(isEnable);
    //侧边栏
    m_sideToolWidget->setEnabled(isEnable);
    //开始键
    ui->startPushButton->setEnabled(isEnable);
    //关闭
    ui->closePushButton->setEnabled(isEnable);
    //停止
    ui->stopPushButton->setEnabled(!isEnable);
}

void Cfhs_MainWindow::functionInit()
{
    //Menu
    m_functionMenu = new QMenu(this);
    //图像显示管理
    m_imageManageAction = new QAction(tr("图像显示管理"), this);
    connect(m_imageManageAction, &QAction::triggered,
            this, &Cfhs_MainWindow::imageManageAction_triggered);
    //图像保存功能
    m_imageSaveAction = new QAction(tr("图像保存功能"), this);
    connect(m_imageSaveAction, &QAction::triggered,
            this, &Cfhs_MainWindow::imageSaveAction_triggered);
    //图像分区设置
    m_imageSubsetAction = new QAction(tr("图像分区设置"), this);
    connect(m_imageSubsetAction, &QAction::triggered,
            this, &Cfhs_MainWindow::imageSubsetAction_triggered);
    m_imgPartitionWidget = new ImgSubSet(this);
    connect(m_imgPartitionWidget, &ImgSubSet::sig_setPoint,
            m_bigImageWidget, &cfhs_mainwindows_img::setPoint);
    connect(m_imgPartitionWidget, &ImgSubSet::sig_setGridView,
            this, &Cfhs_MainWindow::slot_ShowGridView);
    //添加到menu
    m_functionMenu->addAction(m_imageManageAction);
    m_functionMenu->addAction(m_imageSaveAction);
    m_functionMenu->addAction(m_imageSubsetAction);
    //添加到button上
    ui->functionPushButton->setMenu(m_functionMenu);
    //添加事件过滤器
    ui->functionLabel->installEventFilter(this);
}

void Cfhs_MainWindow::helpInit()
{
    //帮助Menu
    m_helpMenu = new QMenu(this);
    //软件版本
    m_softVersionAction = new QAction(tr("软件版本"), this);
    connect(m_softVersionAction, &QAction::triggered,
            this, &Cfhs_MainWindow::softVersionAction_triggered);
    //软件常见问题sop
    m_faultManualAction = new QAction(tr("软件常见问题sop"), this);
    connect(m_faultManualAction, &QAction::triggered,
            this, &Cfhs_MainWindow::faultManualAction_triggered);
    //软件使用说明
    m_softManualAction = new QAction(tr("软件使用说明"),this);
    connect(m_softManualAction, &QAction::triggered,
            this, &Cfhs_MainWindow::softManualAction_triggered);
    //易损件清单
    m_listConsumablesAction = new QAction(tr("易损件清单"), this);
    connect(m_listConsumablesAction, &QAction::triggered,
            this, &Cfhs_MainWindow::listConsumablesAction_triggered);
    //远程协助
    m_teamviewerAction = new QAction(tr("远程协助"), this);
    connect(m_teamviewerAction, &QAction::triggered,
            this, &Cfhs_MainWindow::teamviewerAction_triggered);
    //缺陷打标
    m_defectCallout = new QAction(tr("缺陷打标"), this);
    connect(m_defectCallout, &QAction::triggered,
            this, &Cfhs_MainWindow::defectCallout_triggered);
    //添加到menu中
    m_helpMenu->addAction(m_softVersionAction);
    m_helpMenu->addAction(m_faultManualAction);
    m_helpMenu->addAction(m_softManualAction);
    m_helpMenu->addAction(m_listConsumablesAction);
    m_helpMenu->addAction(m_teamviewerAction);
    m_helpMenu->addAction(m_defectCallout);
    //设置到button上
    ui->helpPushButton->setMenu(m_helpMenu);
    ui->helpLabel->installEventFilter(this);
}

bool Cfhs_MainWindow::setProgram(const stProgramme &stPro)
{
    //设置方案名
    ui->programTextLabel->setText(stPro.strProName);
    //设置扫码枪
    setReadCodingOpen(stPro.bOpenScan);
    //扫码率默认为0
    QString strRate = "0%";
    slot_ShowScanRate(strRate);
    //设置Mes
    setMesReportOpen(stPro.bOpenMes);
    //Mes上传率默认为0
    slot_ShowMesRate(strRate);
    //输出结果
    setResultMode(stPro.iResultMode);
    //结果重置
    m_resultWidget->resultInit();
    //批次表
    QStringList batchList;
    batchList.append(tr("检测批次"));
    batchList.append(tr("检测总量"));
    batchList.append(tr("合格数量"));
    batchList.append(tr("次品数量"));
    batchList.append(tr("产品良率"));
    m_batchTable->setHeaderString(batchList);
    m_batchTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_batchTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    //Roi
    QStringList roiList;
    roiList.append(tr("执行区名称"));
    roiList.append(tr("使用算法汇总"));
    m_roiTable->setHeaderString(roiList);
    m_roiTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_roiTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //良率统计表
    m_batchChart->clearContent();
    //缺陷小图
    m_defectSmallImage[0]->clearContent();
    m_defectSmallImage[1]->clearContent();
    m_defectSmallImage[2]->clearContent();
    //设置九宫格数据
    m_imgPartitionWidget->SetDataUpdate(stPro.strImagePart);

    return true;
}

void Cfhs_MainWindow::setResultMode(const int &mode)
{
    ResultEnum eResult = static_cast<ResultEnum>(mode);
    switch(eResult)
    {
    case ResultNormal:
        ui->normalRadioButton->setChecked(true);
        break;
    case ResultOk:
        ui->okRadioButton->setChecked(true);
        break;
    case ResultNg:
        ui->ngRadioButton->setChecked(true);
        break;
    }
}

void Cfhs_MainWindow::showBigImagePoints(const QPolygon &vectorPoint)
{
    m_bigImageWidget->ClearPoint();
    //添加坐标
    int size = vectorPoint.size();
    for(int i=0; i<size; i++)
    {
        QPoint point = vectorPoint.at(i);
        m_bigImageWidget->addPoint(point);
    }
}

void Cfhs_MainWindow::showBigImage(const QImage &image, const QList<itoPoint> &list)
{
    m_bigImageWidget->setImage(image, list);
}

void Cfhs_MainWindow::showBigImageTotal(const QPolygon &vectorPoint, const QImage &image,
                                        const QList<itoPoint> &list)
{
    //显示缺陷图
    showBigImage(image, list);
    //显示缺陷坐标
    showBigImagePoints(vectorPoint);
}

void Cfhs_MainWindow::slot_ShowFrameInfo(bool bOK, const QString &strInfo)
{
    Q_UNUSED(bOK);
    ShowMessage(strInfo);
}

void Cfhs_MainWindow::slot_ShowInfo(bool bNormal, const QString &strInfo)
{
    //日志添加时间[yyyy-MM-dd  hh:mm:ss]
    QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm:ss");
    QString message = QString("  [%1]   %2").arg(strTime).arg(strInfo);
    m_statusBar->ShowMessageInfo(bNormal, message);
}

void Cfhs_MainWindow::slot_ShowScanRate(const QString &strInfo)
{
    QString info = QString(tr("解码率%1")).arg(strInfo);
    ui->readCodeLabel->setText(info);
}

void Cfhs_MainWindow::slot_ShowMesRate(const QString &strInfo)
{
    QString info = QString(tr("上报率%1")).arg(strInfo);
    ui->mesReportLabel->setText(info);
}

void Cfhs_MainWindow::slot_ShowFeatureData(const int &stationNo, const QString &strData)
{
    if(m_mapStationFeature.contains(stationNo))
        m_mapStationFeature[stationNo] = strData;
    else
        m_mapStationFeature.insert(stationNo, strData);
    //添加坐标
    QList<itoPoint> listPoint;
    if(!strData.isEmpty())
    {
        QStringList allList = strData.split("@");
        foreach(QString info, allList)
        {
            QStringList singleList = info.split("#");
            if(singleList.size() < 6)
                break;
            itoPoint point;
            //起始坐标
            QString strPos = singleList.at(3);
            if(!strPos.contains(","))
                continue;
            int x = strPos.section(",",0,0).toInt();
            int y = strPos.section(",",1).toInt();
            point.Start.setX(x);
            point.Start.setY(y);
            //终点坐标
            strPos = singleList.at(4);
            if(!strPos.contains(","))
                continue;
            x = strPos.section(",",0,0).toInt();
            y = strPos.section(",",1).toInt();
            point.End.setX(x);
            point.End.setY(y);
            listPoint.append(point);
        }
        if(m_mapStationItoPoint.contains(stationNo))
            m_mapStationItoPoint[stationNo] = listPoint;
        else
            m_mapStationItoPoint.insert(stationNo, listPoint);
    }

    //如果是当前工位显示
    if(m_taskWidget->getCurrentTask() == stationNo)
    {
        QStringList listFeature = m_mapStationFeatureName.value(stationNo);
        setFeatureData(listFeature, strData);
        //显示工位图
        QImage image;
        if(m_mapStationImg.contains(stationNo))
            image = m_mapStationImg[stationNo];
        showBigImage(image, listPoint);
    }
}

void Cfhs_MainWindow::slot_ShowBatchFormInfo(const QString &BatchInfo)
{
    m_batchTable->tableInit();
    m_batchTable->addData(BatchInfo);
}

void Cfhs_MainWindow::slot_ShowStationImagePoint(int iStation, const QImage &imageBig,
                                                 const QString &strDefectName1, const QString &strDefectName2,
                                                 const QString &strDefectName3, const QImage &imageSmall1,
                                                 const QImage &imageSmall2, const QImage &imageSmall3,
                                                 const QString &strPoint)
{
    //保存大图
    if(m_mapStationImg.contains(iStation))
        m_mapStationImg[iStation] = imageBig;
    else
        m_mapStationImg.insert(iStation, imageBig);
    //保存缺陷名
    QMap<int, QString> mapName;
    mapName.insert(1, strDefectName1);
    mapName.insert(2, strDefectName2);
    mapName.insert(3, strDefectName3);
    if(m_mapStationDefectName.contains(iStation))
        m_mapStationDefectName[iStation] = mapName;
    else
        m_mapStationDefectName.insert(iStation, mapName);
    //保存缺陷图
    QMap<int, QImage> mapImg;
    mapImg.insert(1, imageSmall1);
    mapImg.insert(2, imageSmall2);
    mapImg.insert(3, imageSmall3);
    if(m_mapStationDefectImg.contains(iStation))
        m_mapStationDefectImg[iStation] = mapImg;
    else
        m_mapStationDefectImg.insert(iStation, mapImg);
    //保存缺陷坐标
    QPolygon polygon = getPolygonFromQString(strPoint);
    if(m_mapStationPoint.contains(iStation))
        m_mapStationPoint[iStation] = polygon;
    else
        m_mapStationPoint.insert(iStation, polygon);

    //显示信息
    if(m_taskWidget->getCurrentTask() == iStation)
    {
        //显示大图
        QList<itoPoint> listPoint;
        if(m_mapStationItoPoint.contains(iStation))
            listPoint = m_mapStationItoPoint[iStation];
        showBigImageTotal(polygon, imageBig, listPoint);
        //显示缺陷小图
        m_defectSmallImage[0]->showDefect(imageSmall1, strDefectName1);
        m_defectSmallImage[1]->showDefect(imageSmall2, strDefectName2);
        m_defectSmallImage[2]->showDefect(imageSmall3, strDefectName3);
    }
    qDebug()<<__FUNCTION__;
}

void Cfhs_MainWindow::slot_ShowResultImagePoint(const QImage &imageBig, const QString &strDefectName1,
                                                const QString &strDefectName2, const QString &strDefectName3,
                                                const QImage &imageSmall1, const QImage &imageSmall2,
                                                const QImage &imageSmall3, const QString &strPoint)
{
    //保存大图
    m_resultImg = imageBig;
    //保存缺陷小图
    if(m_mapResultDefectImg.contains(1))
        m_mapResultDefectImg[1] = imageSmall1;
    else
        m_mapResultDefectImg.insert(1, imageSmall1);
    if(m_mapResultDefectImg.contains(2))
        m_mapResultDefectImg[2] = imageSmall2;
    else
        m_mapResultDefectImg.insert(2, imageSmall2);
    if(m_mapResultDefectImg.contains(3))
        m_mapResultDefectImg[3] = imageSmall3;
    else
        m_mapResultDefectImg.insert(3, imageSmall3);
    //保存缺陷名
    if(m_mapResultDefectName.contains(1))
        m_mapResultDefectName[1] = strDefectName1;
    else
        m_mapResultDefectName.insert(1, strDefectName1);
    if(m_mapResultDefectName.contains(2))
        m_mapResultDefectName[2] = strDefectName2;
    else
        m_mapResultDefectName.insert(2, strDefectName2);
    if(m_mapResultDefectName.contains(3))
        m_mapResultDefectName[3] = strDefectName3;
    else
        m_mapResultDefectName.insert(3, strDefectName3);
    //保存缺陷坐标
    m_polygonResultPoint = getPolygonFromQString(strPoint);
    if(m_taskWidget->getCurrentTask() == Cfhs_TaskInfoWidget::ResultTask)
    {
        //显示大图
        showBigImageTotal(m_polygonResultPoint, m_resultImg);
        //显示缺陷小图
        for(int i=0; i<3; i++)
        {
            QImage img = m_mapResultDefectImg.value(i+1);
            QString name = m_mapResultDefectName.value(i+1);
            m_defectSmallImage[i]->showDefect(img, name);
        }
    }
}

void Cfhs_MainWindow::slot_ShowAnalysisImagePoint(const QImage &imageBig, const QString &strPoint)
{
    //保存大图
    m_analysisImg = imageBig;
    //保存缺陷坐标
    m_polygonAnalysisPoint = getPolygonFromQString(strPoint);
    if(m_taskWidget->getCurrentTask() == Cfhs_TaskInfoWidget::AnalysisTask)
    {
        //显示大图
        showBigImageTotal(m_polygonAnalysisPoint, m_analysisImg);
    }
}

void Cfhs_MainWindow::slot_ShowBatchChartInfo(const QString &strHourName, const QString &strHourInput, const QString &strHourYield, const QString &strHourTotalYield)
{
    m_batchChart->setData(strHourName, strHourInput, strHourYield, strHourTotalYield);
}

void Cfhs_MainWindow::slot_ShowDiskSpaceAlarm(const QString &strInfo)
{
    ShowMessage(strInfo);
}

void Cfhs_MainWindow::slot_ShowYieldAlarm(const QString &strInfo)
{
    ShowMessage(strInfo);
}

void Cfhs_MainWindow::slot_ShowCurrentTask(const int &index)
{
    //是否显示九宫格 ，只在结果页面显示
    bool isShowGridView = false;
    QString strFeather = "";
    QStringList listFeature;
    if(index == Cfhs_TaskInfoWidget::ResultTask)
    {
        //显示结果信息
        //显示大图
        showBigImageTotal(m_polygonResultPoint, m_resultImg);
        //显示缺陷图
        for(int i=0; i<3; i++)
        {
            QString name = m_mapResultDefectName.value(i+1);
            QImage img = m_mapResultDefectImg.value(i+1);
            m_defectSmallImage[i]->showDefect(img, name);
        }
        isShowGridView = m_openGridview;
    }
    else if(index == Cfhs_TaskInfoWidget::AnalysisTask)
    {
        //显示分析界面
        //显示大图
        showBigImageTotal(m_polygonAnalysisPoint, m_analysisImg);
        //缺陷图清空
        for(int i=0; i<3; i++)
            m_defectSmallImage[i]->clearContent();
    }
    else
    {
        //显示工位界面
        //显示大图
        QImage img = m_mapStationImg.value(index);
        QPolygon polygon = m_mapStationPoint.value(index);
        QList<itoPoint> points = m_mapStationItoPoint.value(index);
        showBigImageTotal(polygon, img, points);
        //显示缺陷图
        for(int i=0; i<3; i++)
        {
            QString name = m_mapStationDefectName.value(index).value(i+1);
            QImage img = m_mapStationDefectImg.value(index).value(i+1);
            m_defectSmallImage[i]->showDefect(img, name);
        }
        strFeather = m_mapStationFeature.value(index);
        listFeature = m_mapStationFeatureName.value(index);
    }
    //显示缺陷特征
    setFeatureData(listFeature, strFeather);
    //显示九宫格
    m_bigImageWidget->setGridView(isShowGridView, m_xGridview, m_yGridview);
}

void Cfhs_MainWindow::slot_ShowGridView(bool open_gridview, int x_gridview, int y_gridview)
{
    m_openGridview = open_gridview;
    m_xGridview = x_gridview;
    m_yGridview = y_gridview;
    if(m_taskWidget->getCurrentTask() == Cfhs_TaskInfoWidget::ResultTask)
        m_bigImageWidget->setGridView(m_openGridview, m_xGridview, m_yGridview);
}

void Cfhs_MainWindow::slot_closeButton_timeout()
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                       tr("关闭"),
                                       tr("确定关闭软件和电脑？"),
                                       QMessageBox::Yes|
                                       QMessageBox::No,
                                       this);
    if(msg->exec() == QMessageBox::Yes)
    {
        m_logicInterface->CloseAll();
    }
    //关闭定时器
    m_closeTimer->stop();
}

void Cfhs_MainWindow::slot_ShowSystemRunStatus(const bool &run)
{
    //设置系统状态
    if(run != m_isSystemStatus)
        setSystemStatus(run);
}

void Cfhs_MainWindow::setUser(const UserType &user)
{
    ui->userTextLabel->setText(user.m_name);
    bool isEnable = true;
    if(user.m_Permission == OperatorPermission)
        isEnable = false;
    setFunctionEnable(isEnable);
}

QString Cfhs_MainWindow::getFunctionButtonStyle(const QString &name, const bool &hasDisable)
{
    QString normalPath = "";
    QString pressPath = "";
    QString disablePath = "";
    if(hasDisable)
    {
        normalPath = QString(":/%1_normal.png").arg(name);
        pressPath = QString(":/%1_press.png").arg(name);
        disablePath = QString(":/%1_disable.png").arg(name);
    }
    else
    {
        normalPath = QString(":/%1_normal.png").arg(name);
        pressPath = QString(":/%1_press.png").arg(name);
        disablePath = normalPath;
    }
    QString style = QString("QPushButton{border-image:url(%1); background:transparent}"
                            "QPushButton:pressed{border-image:url(%2);}"
                            "QPushButton:disabled{border-image:url(%3); background:#3954C9}"
                            "QPushButton:menu-indicator{image:none}")
                            .arg(normalPath)
                            .arg(pressPath)
                            .arg(disablePath);


    return style;
}

bool Cfhs_MainWindow::ReadProgram()
{
    //获取当前方案名
    stConfig conf;
    QString strInfo;
    if(!m_logicInterface->GetConfigInfo(conf, strInfo))
    {
        QMessageBox::information(this, " ", strInfo);
        return false;
    }
    m_curProgramName = conf.strNowProName;
    if(m_curProgramName.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("当前没有可执行方案"));
        return false;
    }
    //获取当前方案
    stProgramme stPro;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, true, strInfo))
    {
        QMessageBox::information(this, " ", strInfo);
        return false;
    }
    //设置当前方案
    if(!setProgram(stPro))
        return false;
    //获取当前工位信息
    QString strAllStation;
    if(!m_logicInterface->GetAllStationNo(m_curProgramName, strAllStation, strInfo))
    {
        QMessageBox::information(this, " ", strInfo);
        return false;
    }
    int stationNum = getListFromQString(strAllStation).size();
    //加载工位
    m_taskWidget->setTask(stationNum);
    //产品特征表
    m_mapStationFeatureName.clear();
    for(int i=0; i<stationNum; i++)
    {
        Station station;
        int stationNo = i + 1;
        if(!m_logicInterface->GetStationInfo(m_curProgramName, stationNo, station, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return false;
        }
        QStringList listFeature;
        if(!getFeatureData(station.strFeatures, listFeature, strInfo))
        {
            strInfo = QString(tr("工位%1的缺陷特征获取失败：%2")).arg(stationNo).arg(strInfo);
            QMessageBox::warning(this, " ", strInfo);
            return false;
        }
        m_mapStationFeatureName.insert(stationNo, listFeature);
    }
    QStringList listFeatureName = m_mapStationFeatureName.value(m_taskWidget->getCurrentTask());
    m_featureTable->setHeaderString(listFeatureName);

    return true;
}


bool Cfhs_MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    //关闭QDialog会话窗时只隐藏窗口，不直接关闭delete
    if(event->type() == QEvent::Close)
    {
        QDialog *dialog = static_cast<QDialog*>(obj);
        dialog->reject();
        event->ignore();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        if(obj == ui->functionLabel)
        {
            ui->functionPushButton->click();
        }
        else if(obj == ui->controlLabel)
        {
            ui->controlPushButton->click();
        }
        else if(obj == ui->settingLabel)
        {
            ui->settingPushButton->click();
        }
        else if(obj == ui->helpLabel)
        {
            ui->helpPushButton->click();
        }
        else if(obj == ui->closePushButton)
        {
            m_closeTimer->start();
        }
        return false;
    }
    else if(event->type() == QEvent::MouseButtonRelease && obj == ui->closePushButton)
    {
        m_closeTimer->stop();
        return false;
    }
    else if(m_taskWidget->getCurrentTask() == Cfhs_TaskInfoWidget::AnalysisTask
            && obj == m_bigImageWidget
            && event->type() == QEvent::MouseButtonDblClick
            && !m_isSystemStatus)
    {
        //分析界面时，可设置统计缺陷点的时间段//默认显示前12小时数据
        QString curText = QString(tr("前%1小时")).arg(m_sumAnalysisTime);
        QString name = tr("统计缺陷点的时间段");
        QStringList list;
        list.append(tr("前1小时"));
        list.append(tr("前6小时"));
        list.append(tr("前8小时"));
        list.append(tr("前12小时"));
        list.append(tr("前24小时"));
        Cfhs_MessageSelect msg(name, list, this);
        msg.setCurrentText(curText);
        QCursor cursor;
        msg.setGeometry(cursor.pos().x(), cursor.pos().y(), 320, 160);
        if(msg.exec() == QDialog::Accepted)
        {
            int time = 0;
            int index = msg.getCurrentIndex();
            switch(index)
            {
            case 0:
                time = 1;
                break;
            case 1:
                time = 6;
                break;
            case 2:
                time = 8;
                break;
            case 3:
                time = 12;
                break;
            case 4:
                time = 24;
                break;
            default:
                time = 12;
                break;
            }
            QString strPoint;
            QString strInfo;
            //获取汇总缺陷点
            if(m_logicInterface->GetAnalysisByHour(time, strPoint, strInfo))
            {
                QImage img = m_analysisImg;
                QPolygon points = getPolygonFromQString(strPoint);
                //显示图片和缺陷点
                showBigImageTotal(points, img);
                //保存时间段
                m_sumAnalysisTime = time;
            }
            else
                QMessageBox::warning(this, " ", strInfo);
        }
        return false;
    }
    else {
        return false;
    }
}

void Cfhs_MainWindow::keyPressEvent(QKeyEvent *event)
{
    static QString strEdit;
    if(event->text() =="c"||event->text() =="C")
        strEdit = event->text();
    else
        strEdit.append(event->text());
    //转换为大写
    strEdit = strEdit.toUpper();
    if(strEdit == "CFHS")
    {
        QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                           tr("提示"),
                                           tr("是否显示附加功能？"),
                                           QMessageBox::Yes|
                                           QMessageBox::No,
                                           this);
        msg->setButtonText(QMessageBox::Yes, tr("显示"));
        msg->setButtonText(QMessageBox::No, tr("隐藏"));
        msg->setDefaultButton(QMessageBox::Yes);
        bool isShowed = false;
        if(msg->exec() == QMessageBox::Yes)
            isShowed = true;
        showAdditionalFunction(isShowed);
        //重置strEdit
        strEdit.clear();
        //释放msg
        delete msg;
        msg = nullptr;
    }
}

void Cfhs_MainWindow::on_closePushButton_clicked()
{
    this->close();
}

void Cfhs_MainWindow::on_startPushButton_clicked()
{
    //获取当前批次名
    stConfig conf;
    QString strInfo;
    if(!m_logicInterface->GetConfigInfo(conf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //批次列表
    QString strAllBatch;
    if(!m_logicInterface->GetAllBatchName(strAllBatch, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QStringList listBatch = getListFromQString(strAllBatch);
    Cfhs_StartBatchDetect dialog(this);
    dialog.setBatchInfo(conf.strNowBatchName, listBatch);
    if(dialog.exec() == QDialog::Accepted)
    {
        ui->startPushButton->setEnabled(false);
        ShowMessage(tr("设备启动中，请稍后..."));
        //侧边栏隐藏
        if(m_sideToolWidget->isSideShow())
            m_sideToolWidget->setSideShow(false);
        //开始
        m_logicInterface->Start();
        m_showMessage->hide();
    }
}

void Cfhs_MainWindow::diskMonitorAction_triggered()  //硬盘监控
{
    stConfig config;
    QString strInfo;
    if(!m_logicInterface->GetConfigInfo(config, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    Cfhs_DiskMonitor disk(this);
    disk.setDiskMonitor(config.iDiskSpaceAlarm, config.iDiskSpaceDelete);
    if(disk.exec() == QDialog::Accepted)
    {
        disk.getDiskMonitor(config.iDiskSpaceAlarm, config.iDiskSpaceDelete);
        if(m_logicInterface->SetConfigInfo(config, strInfo))
            QMessageBox::information(this, " ", tr("设置成功"));
        else
            QMessageBox::warning(this, " ", strInfo);
    }
}

void Cfhs_MainWindow::softMaintainAction_triggered()  //软件维护
{
    Cfhs_SoftMaintain soft(this);
    soft.exec();
}

void Cfhs_MainWindow::yieldWarningAction_triggered()  //实时良率预警
{
    stConfig config;
    QString strInfo;
    if(!m_logicInterface->GetConfigInfo(config, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    Cfhs_YieldWarning yield(this);
    yield.setYieldValue(config.iYieldAlarmNum, config.strYieldAlarm);
    if(yield.exec() == QDialog::Accepted)
    {
        yield.getYieldValue(config.iYieldAlarmNum, config.strYieldAlarm);
        if(m_logicInterface->SetConfigInfo(config, strInfo))
            QMessageBox::information(this, " ", tr("设置成功"));
        else
            QMessageBox::warning(this, " ", strInfo);
    }
}

void Cfhs_MainWindow::programConfigAction_triggered() //方案配置
{
    if(!m_programConfigWidget)
    {
        m_programConfigWidget = new Cfhs_ProgramConfig(this);
    }
    m_programConfigWidget->show();
    if(m_programConfigWidget->windowState() == Qt::WindowMinimized)
        m_programConfigWidget->setWindowState(Qt::WindowNoState);
}

void Cfhs_MainWindow::imageSpliceAction_triggered()  //图像拼接管理
{

}

void Cfhs_MainWindow::signalNgAction_triggered() // NG/OK信号管理
{

}

void Cfhs_MainWindow::addCameraAction_triggered()
{
    Cfhs_AddCameraInfo camera(this);
    if(camera.DialogShow() == QDialog::Accepted)
    {
        QMessageBox::information(this, " ", tr("相机记录添加成功"));
    }
}

void Cfhs_MainWindow::imageManageAction_triggered()
{
    Img_Show_Managent dialog(this);
    if(dialog.DialogShow() == QDialog::Accepted)
    {
        //读取当前方案
        QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                           tr("提示"),
                                           tr("方案读取中，请稍后..."),
                                           QMessageBox::Ok,
                                           this);
        msg->show();
        Sleep(10);
        if(ReadProgram())
        {
            msg->close();
            QMessageBox::information(this, " ", tr("方案选择成功"));
        }
        msg->close();
        delete msg;
        msg = nullptr;
    }
}

void Cfhs_MainWindow::imageSaveAction_triggered()
{
    ImgSaveMode img(this);
    img.DialogShow();
}

void Cfhs_MainWindow::imageSubsetAction_triggered()
{
    m_imgPartitionWidget->DialogShow();
}

void Cfhs_MainWindow::softVersionAction_triggered()
{
    Cfhs_SoftwareVersion soft(this);

    soft.exec();
}

void Cfhs_MainWindow::faultManualAction_triggered()
{

}

void Cfhs_MainWindow::softManualAction_triggered()
{
    QString path = QCoreApplication::applicationDirPath() + "/Resource/";
    switch(m_currentLang)
    {
    case English:
        path += "help_En.doc";
        break;
    case SimplifiedChinese:
        path += "help_Chs.doc";
        break;
    case TraditionalChinese:
        path = "help_Cht.doc";
        break;
    }
    QDesktopServices::openUrl(QUrl(path));
}

void Cfhs_MainWindow::listConsumablesAction_triggered()
{

}

void Cfhs_MainWindow::teamviewerAction_triggered()
{

}

void Cfhs_MainWindow::defectCallout_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("选择缺陷图"),
                                                "",
                                                "Image(*.png *.bmp *.jpg)");
    if(path.isEmpty())
        return;
    QString strDir = Cfhs_SideTool::getDirFromFilePath(path);
    QDir dir;
    dir.setPath(strDir);
    if(!dir.exists())
    {
        QMessageBox::warning(this, " ", tr("缺陷图路径错误，请重新选择"));
        return;
    }
    Cfhs_DefectCallout callout(strDir, this);
    if(callout.exec() == QDialog::Accepted)
    {

    }
}

//显示系统状态
void Cfhs_MainWindow::setSystemStatus(const bool& isStatused)
{
    m_isSystemStatus = isStatused;
    //设置label固定的大小
    QSize size = QSize(64, 64);
    if(ui->sysStatusLabel->size() != size)
        ui->sysStatusLabel->setFixedSize(size);
    //清空lable的text
    if(!ui->sysStatusLabel->text().isEmpty())
        ui->sysStatusLabel->setText("");
    QString style;
    if(isStatused)
        style = QString("border-image: url(:/normal_status.png)");
    else
        style = QString("border-image: url(:/abnormal_status.png)");

    ui->sysStatusLabel->setStyleSheet(style);
    //异常状态时停止系统
    if(!isStatused)
        m_logicInterface->Stop();
    //功能按键启用
    setFunctionEnable(!isStatused);
}

//显示工位状态
// 0 待机，1 成功， 2 异常
void Cfhs_MainWindow::setStationStatus(QPushButton *button, const int& status)
{
    //设置固定大小
    QSize size = QSize(100, 34);
    if(button->size() != size)
        button->setFixedSize(size);
    //设置状态
    QString style;
    switch(status)
    {
    case 0:
        style = QString("background:#999999; border:none; border-radius:14px;");
        break;
    case 1:
        style = QString("background:#037200; border:none; border-radius:14px;");
        break;
    case 2:
        style = QString("background:#D42E2E; border:none; border-radius:14px;");
        break;
    default:
        style = QString("background:#999999; border:none; border-radius:14px;");
        break;
    }

    button->setStyleSheet(style);
}

//读码开关
void Cfhs_MainWindow::setReadCodingOpen(const bool& isOpened)
{
    QSize size = QSize(75, 34);
    //设置固定大小
    if(ui->readCodePushButton->size() != size)
        ui->readCodePushButton->setFixedSize(size);
    //清空text
    if(!ui->readCodePushButton->text().isEmpty())
        ui->readCodePushButton->setText("");
    m_isReadCoded = isOpened;
    //设置显示状态
    QString style = switch_button::getSwitchButtonStyle(m_isReadCoded);

    ui->readCodePushButton->setStyleSheet(style);
}

//MES上传开关
void Cfhs_MainWindow::setMesReportOpen(const bool& isOpened)
{
    QSize size = QSize(75, 34);
    //设置固定大小
    if(ui->mesReportPushButton->size() != size)
        ui->mesReportPushButton->setFixedSize(size);
    //清空text
    if(ui->mesReportPushButton->text().isEmpty())
        ui->mesReportPushButton->setText("");
    m_isMesReported = isOpened;
    //设置显示状态
    QString style = switch_button::getSwitchButtonStyle(m_isMesReported);

    ui->mesReportPushButton->setStyleSheet(style);
}

void Cfhs_MainWindow::showAdditionalFunction(const bool &isShowed)
{
    //添加相机记录
    m_addCameraAction->setVisible(isShowed);
    //如果显示附加功能则弹出设置menu
    if(isShowed)
        ui->settingPushButton->click();
}

void Cfhs_MainWindow::createData()
{
    //大图、缺陷点
    QPolygon listPoint;
    listPoint.append(QPoint(9470, 3200));
    listPoint.append(QPoint(3600, 8560));
    listPoint.append(QPoint(5840, 17820));
    showBigImageTotal(listPoint, m_resultImg);
    //缺陷小图
    //1
    QString strName = "黑点";
    QImage img;
    img.load(":/image2.png");
    m_defectSmallImage[0]->showDefect(img, strName);
    //2
    strName = "黑线";
    img.load(":/image1.png");
    m_defectSmallImage[1]->showDefect(img, strName);
    //3
    strName = "白线";
    img.load(":/image4.png");
    m_defectSmallImage[2]->showDefect(img, strName);
    //结果图
    QString strCurRel = "OK";
    emit sig_ShowNowResult(strCurRel);
    QString strPrevRel = "OK#OK#NG#OK#OK#OK#OK#OK#NG#OK";
    strPrevRel.append("#NG#OK#OK#OK#OK#OK#OK#OK#OK#OK");
    emit sig_Show20Result(strPrevRel);
    //批次数据
    QString strBatch = "1910091806_A_01_0002#252#220#30#87.3%";
    m_batchTable->addData(strBatch);
    //特征表
    //QString strFeat1 = "1910091809#2#黑点#3600,8560#100.2#200.3#20#30#40#10#20#12.5";
    //QString strFeat2 = "1910091817#1#黑线#9470,3200#200.2#400.3#18#330#34#10#30#32.5";
    //QString strFeat3 ="1910091817#3#白线#5840,17820#220.2#360.3#25#430#24#30#27#72.5";
    //m_featureTable->addData(strFeat1);
    //m_featureTable->addData(strFeat2);
    //m_featureTable->addData(strFeat3);
    QString strFeat1 = "1910091809#2#黑点#0,0#3600,8560#10000";
    slot_ShowFeatureData(1, strFeat1);
    //良率表
    QString strHour = "7#8#9#10#11#12#13#14#15#16#17#18";
    QString strInput = "200#789#658#549#657#895#788#864#765#698#655#821";
    QString strYield = "78#50#69#78#89#52#45#64#42#68#55#96";
    QString strTotal = "75#68#82#80#76#74#72#84#87#89#90#65";
    m_batchChart->setData(strHour,strInput,strYield,strTotal);
}

void Cfhs_MainWindow::setFeatureData(const QStringList &listFeatureName, const QString &strFeature)
{
    m_featureTable->setHeaderString(listFeatureName);
    if(strFeature.isEmpty())
        return;
    QStringList list = strFeature.split("@");
    for(int i=0; i<list.size(); i++)
        m_featureTable->addData(list.at(i));
}

bool Cfhs_MainWindow::getFeatureData(const QString &strFeature,
                                     QStringList &listFeature, QString &strInfo)
{
    QMap<QString, QString> mapFeature;
    if(!getMapFromJson(strFeature, mapFeature, strInfo))
        return false;
    if(mapFeature.isEmpty())
    {
        strInfo = QString(tr("数据为空"));
        return false;
    }
    //选择要显示的语言版本
    QString showFeature;
    switch(m_currentLang)
    {
    case English:
        showFeature = mapFeature.value("English");
        break;
    case SimplifiedChinese:
        showFeature = mapFeature.value("Simplified");
        break;
    case TraditionalChinese:
        showFeature = mapFeature.value("Traditional");
        break;
    }
    QStringList list = getListFromQString(showFeature);
    if(list.isEmpty())
    {
        strInfo = QString(tr("数据为空"));
        return false;
    }
    //去掉距离和个数
    if(m_currentLang == SimplifiedChinese)
    {
        list.removeOne("距离");
        list.removeOne("个数");
    }
    else if(m_currentLang == TraditionalChinese)
    {
        list.removeOne("距離");
        list.removeOne("個數");
    }
    else
    {
        list.removeOne("DefectDis");
        list.removeOne("DefectNum");
    }
    //默认添加“产品ID”，“缺陷排序ID”，“缺陷名称”
    QStringList realFeatureList;
    if(m_currentLang == English)
    {
        realFeatureList.append("Product ID");
        realFeatureList.append("Defect ID");
        realFeatureList.append("Defect name");
    }
    else if(m_currentLang == SimplifiedChinese)
    {
        realFeatureList.append("产品ID");
        realFeatureList.append("缺陷排序ID");
        realFeatureList.append("缺陷名称");
    }
    else
    {
        realFeatureList.append("產品ID");
        realFeatureList.append("缺陷排序ID");
        realFeatureList.append("缺陷名稱");
    }
    realFeatureList.append(list);
    listFeature = realFeatureList;
    return true;
}

void Cfhs_MainWindow::on_readCodePushButton_clicked()
{
    bool isStatus = !m_isReadCoded;
    stProgramme stPro;
    QString strInfo;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, false, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    stPro.bOpenScan = isStatus;
    if(m_logicInterface->SetProInfo(m_curProgramName, stPro, strInfo))
        setReadCodingOpen(isStatus);
    else
        QMessageBox::warning(this, " ", strInfo);
}

void Cfhs_MainWindow::on_mesReportPushButton_clicked()
{
    bool isStatus = !m_isMesReported;
    stProgramme stPro;
    QString strInfo;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, false, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    stPro.bOpenMes = isStatus;
    if(m_logicInterface->SetProInfo(m_curProgramName, stPro, strInfo))
        setMesReportOpen(isStatus);
    else
        QMessageBox::warning(this, " ", strInfo);
}

void Cfhs_MainWindow::on_programPushButton_clicked()
{
    QString strInfo;
    //获取方案列表
    QString programInfo;
    if(!m_logicInterface->GetAllProName(programInfo, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QString name = tr("方案名");
    QStringList list = programInfo.split("#");  //方案列表
    Cfhs_MessageSelect msg(name, list, this);
    if(msg.exec() == QDialog::Accepted)
    {
        QString curProgram = msg.getMessage();
        //设置当前方案
        stConfig conf;
        if(!m_logicInterface->GetConfigInfo(conf, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        conf.strNowProName = curProgram;
        if(!m_logicInterface->SetConfigInfo(conf, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        //读取当前方案
        QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                           tr("提示"),
                                           tr("方案读取中，请稍后..."),
                                           QMessageBox::Ok,
                                           this);
        msg->show();
        Sleep(10);
        if(ReadProgram())
        {
            msg->close();
            QMessageBox::information(this, " ", tr("方案选择成功"));
        }
        msg->close();
        delete msg;
        msg = nullptr;
    }
}

void Cfhs_MainWindow::on_stopPushButton_clicked()
{
    m_logicInterface->Stop();
}

void Cfhs_MainWindow::on_connectPushButton_clicked()
{

}

void Cfhs_MainWindow::on_normalRadioButton_clicked()
{
    stProgramme stPro;
    QString strInfo;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, false, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //备份旧的输出模式
    int oldMode = stPro.iResultMode;
    stPro.iResultMode = static_cast<int>(ResultNormal);
    if(!m_logicInterface->SetProInfo(m_curProgramName, stPro, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        setResultMode(oldMode);
    }
}

void Cfhs_MainWindow::on_okRadioButton_clicked()
{
    stProgramme stPro;
    QString strInfo;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, false, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //备份旧的输出模式
    int oldMode = stPro.iResultMode;
    stPro.iResultMode = static_cast<int>(ResultOk);
    if(!m_logicInterface->SetProInfo(m_curProgramName, stPro, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        setResultMode(oldMode);
    }
}

void Cfhs_MainWindow::on_ngRadioButton_clicked()
{
    stProgramme stPro;
    QString strInfo;
    if(!m_logicInterface->GetProInfo(m_curProgramName, stPro, false, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //备份旧的输出模式
    int oldMode = stPro.iResultMode;
    stPro.iResultMode = static_cast<int>(ResultNg);
    if(!m_logicInterface->SetProInfo(m_curProgramName, stPro, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        setResultMode(oldMode);
    }
}
