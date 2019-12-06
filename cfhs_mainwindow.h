#ifndef CFHS_MAINWINDOW_H
#define CFHS_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QEvent>
#include <QMessageBox>
#include <QMap>
#include <QPolygon>
#include <QActionGroup>
#include <QKeyEvent>
#include <QTimer>
#include "cfhs_tablewidget.h"
#include "cfhs_statusbar.h"
#include "cfhs_productresultwidget.h"
#include "cfhs_defectsmallimage.h"
#include "cfhs_barchart.h"
#include "cfhs_sidetool.h"
#include "cfhs_startbatchdetect.h"
#include "Control/cfhs_diskmonitor.h"
#include "Control/cfhs_softmaintain.h"
#include "Control/cfhs_yieldwarning.h"
#include "Setting/cfhs_programconfig.h"
#include "cfhs_base.h"
#include "cfhs_global.h"
#include "cfhs_mainwindows_img.h"
#include "cfhs_showzoomimage.h"
#include "cfhs_taskinfowidget.h"
#include "Function/imgsubset.h"



namespace Ui {
class Cfhs_MainWindow;
}

class Cfhs_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ResultEnum{ResultNormal=1, ResultOk, ResultNg};
    explicit Cfhs_MainWindow(QWidget *parent = nullptr);
    ~Cfhs_MainWindow();
    void setUser(const UserType &user);
    static QString getFunctionButtonStyle(const QString& name, const bool& hasDisable = false);
    bool ReadProgram();

protected:
    bool eventFilter(QObject* obj, QEvent* event);
    void keyPressEvent(QKeyEvent *event);

private:
    void controlInit();  //控制模块初始化
    void settingInit();  //设置模块初始化
    void setWindowStyle();  //设置主界面Ui
    void mainWindowInit();
    //设置功能键是否启用，操作员、软件运行时禁用
    void setFunctionEnable(const bool &isEnable);
    void functionInit();  //功能模块初始化
    void helpInit(); //帮助模块初始化
    //更新界面方案信息
    bool setProgram(const stProgramme &stPro);
    //设置结果输出模式
    void setResultMode(const int &mode);
    //显示大图及缺陷点
    void showBigImage(const QImage &image, const QPolygon &vectorPoint);
    //显示系统状态
    void setSystemStatus(const bool& isStatused);
    //显示工位状态
    // 0 待机，1 成功， 2 异常
    void setStationStatus(QPushButton *button, const int& status);
    //读码开关
    void setReadCodingOpen(const bool& isOpened);
    //MES上传开关
    void setMesReportOpen(const bool& isOpened);
    //显示附加功能，只供内部人员使用
    void showAdditionalFunction(const bool &isShowed);
    //模拟数据
    void createData();
    //设置缺陷特征信息
    void setFeatureData(const QString &strFeature);

signals:
    //---显示当前产品结果
    //输入
    //strResult:结果信息(取值：OK代表良品,NG代表不良品)
    void sig_ShowNowResult(const QString &strResult);

    //---显示前20个的结果
    //输入
    //strResult:结果信息（取值格式：第1个结果#第2个结果...#第20个结果）(取值：OK代表良品,NG代表不良品)
    void sig_Show20Result(const QString &strResult);

    //---显示工位连接状态
    //输入
    //iStationNo:工位号
    //iConnState:连接状态(取值：1代表已连接,2代表未连接,3代表待机)
    void sig_ShowStationStatus(int iStationNo, int iConnState);

public slots:
    //---显示弹框信息
    //bOK:离线模式执行结果
    //strInfo:弹框信息
    void slot_ShowFrameInfo(bool bOK, const QString &strInfo);

    //---显示日志信息
    //bNormal:true-正常信息，白色字体;false-异常信息，红色字体
    //strInfo:执行信息
    void slot_ShowInfo(bool bNormal,const QString &strInfo);

    //---显示扫码率
    //输入
    //strInfo:扫码率
    void slot_ShowScanRate(const QString &strInfo);

    //---显示Mes上报率
    //输入
    //strInfo:上报率
    void slot_ShowMesRate(const QString &strInfo);

    //---显示特征信息
    //输入
    //strData:特征信息（取值格式：产品ID#缺陷ID#缺陷名称#特征1#特征2...#特征N@产品ID#缺陷ID#缺陷名称#特征1#特征2...#特征N）
    void slot_ShowFeatureData(const int &stationNo, const QString &strData);

    //---显示批次表单信息
    //输入
    //BatchInfo:批次表单信息（取值格式：检测批次#检测总量#合格数量#次品数量#产品良率）
    void slot_ShowBatchFormInfo(const QString &BatchInfo);

    //---显示工位图片和缺陷点
    //输入
    //iStation:工位号
    //imageBig:需显示的大图
    //strDefectName1：需显示的缺陷1号缺陷名
    //strDefectName2：需显示的缺陷2号缺陷名
    //strDefectName3：需显示的缺陷3号缺陷名
    //imageSmall1:需显示的缺陷1号小图
    //imageSmall2:需显示的缺陷2号小图
    //imageSmall3:需显示的缺陷3号小图
    //strPoint:需显示在大图上的缺陷点（取值格式：X坐标1,Y坐标1#X坐标2,Y坐标2#...X坐标N,Y坐标N）
    void slot_ShowStationImagePoint(int iStation, const QImage &imageBig, const QString &strDefectName1, const QString &strDefectName2, const QString &strDefectName3, const QImage &imageSmall1, const QImage &imageSmall2, const QImage &imageSmall3, const QString &strPoint);

    //---显示结果图片和缺陷点
    //输入
    //imageBig:需显示的大图
    //strDefectName1：需显示的缺陷1号缺陷名
    //strDefectName2：需显示的缺陷2号缺陷名
    //strDefectName3：需显示的缺陷3号缺陷名
    //imageSmall1:需显示的缺陷1号小图
    //imageSmall2:需显示的缺陷2号小图
    //imageSmall3:需显示的缺陷3号小图
    //strPoint:需显示在大图上的缺陷点（取值格式：X坐标1,Y坐标1#X坐标2,Y坐标2#...X坐标N,Y坐标N）
    void slot_ShowResultImagePoint(const QImage &imageBig, const QString &strDefectName1, const QString &strDefectName2, const QString &strDefectName3, const QImage &imageSmall1, const QImage &imageSmall2, const QImage &imageSmall3, const QString &strPoint);

    //---显示分析图片和缺陷点
    //输入
    //imageBig:需显示的大图
    //strPoint:需显示在大图上的缺陷点（取值格式：X坐标1,Y坐标1#X坐标2,Y坐标2#...X坐标N,Y坐标N）
    void slot_ShowAnalysisImagePoint(const QImage &imageBig, const QString &strPoint);

    //---显示批次图表信息
    //输入
    //strHourName:小时名字（取值格式：小时名1#小时名2...#小时名12）
    //strHourInput:每小时投入（取值格式：小时投入1#小时投入2...#小时投入12）
    //strHourYield:每小时良率（取值格式：小时良率1#小时良率2...#小时良率12）
    //strHourTotalYield:每小时累积良率（取值格式：小时累积良率1#小时累积良率2...#小时累积良率12）
    void slot_ShowBatchChartInfo(const QString &strHourName, const QString &strHourInput, const QString &strHourYield, const QString &strHourTotalYield);

    //---磁盘监控报警
    //strInfo:信息
    void slot_ShowDiskSpaceAlarm(const QString &strInfo);

    //---良率报警
    //strInfo:信息
    void slot_ShowYieldAlarm(const QString &strInfo);
    //显示当前工位信息
    //index:工位索引
    void slot_ShowCurrentTask(const int &index);
    //开启九宫格
    void slot_ShowGridView(bool open_gridview,int x_gridview,int y_gridview);
    //关闭按钮按下超过3s，关闭软件
    void slot_closeButton_timeout();
    //显示系统状态
    void slot_ShowSystemRunStatus(const bool &run);

private:
    //控制模块
    void diskMonitorAction_triggered();  //硬盘监控
    void softMaintainAction_triggered();  //软件维护
    void yieldWarningAction_triggered();  //实时良率预警
    //设置模块
    void programConfigAction_triggered(); //方案配置
    void imageSpliceAction_triggered();  //图像拼接管理
    void signalNgAction_triggered();   //NG/OK信号管理
    void addCameraAction_triggered();  //添加相机记录
    //功能模块
    void imageManageAction_triggered(); //图像显示管理
    void imageSaveAction_triggered();  //图像保存功能
    void imageSubsetAction_triggered();  //图像分区设置
    //帮助模块
    void softVersionAction_triggered(); //软件版本
    void faultManualAction_triggered(); //常用问题sop
    void softManualAction_triggered(); //软件使用说明书
    void listConsumablesAction_triggered(); //易损件清单
    void teamviewerAction_triggered();  //远程协助
    void defectCallout_triggered();    //缺陷打标

private slots:
    void on_closePushButton_clicked();  //点击关闭按钮
    void on_startPushButton_clicked();  //开始批次检测
    void on_readCodePushButton_clicked();  //读码开关
    void on_mesReportPushButton_clicked();  //MES上传开关
    void on_programPushButton_clicked();  //选择方案
    void on_stopPushButton_clicked();  //暂停
    void on_connectPushButton_clicked(); //重连
    void on_normalRadioButton_clicked();  //输出正常
    void on_okRadioButton_clicked();   //强制OK
    void on_ngRadioButton_clicked();  //强制NG

private:
    Ui::Cfhs_MainWindow *ui;
    QPointer<Cfhs_TableWidget> m_featureTable;  //特征信息表
    QPointer<Cfhs_TableWidget> m_batchTable;  //批次表单
    QPointer<Cfhs_StatusBar> m_statusBar;  //状态栏
    QPointer<Cfhs_ProductResultWidget> m_resultWidget;  //产品结果显示
    QPointer<Cfhs_DefectSmallImage> m_defectSmallImage[3];  //3张缺陷小图
    QPointer<Cfhs_Barchart> m_batchChart;   //统计图表
    QPointer<Cfhs_SideTool> m_sideToolWidget; //侧边工具栏
    bool m_isReadCoded;  //读码开关
    bool m_isMesReported;  //MES上传开关
    QPointer<QTimer> m_closeTimer;  //关闭软件和电脑定时器
    bool m_isSystemStatus; //系统运行状态
    QPointer<QMessageBox> m_showMessage;  //显示弹出信息
    QPointer<cfhs_mainwindows_img> m_bigImageWidget;  //大图
    QPointer<Cfhs_ShowZoomImage> m_zoomImageWidget; // 放大图
    QPointer<Cfhs_TaskInfoWidget> m_taskWidget;  //工位栏
    QPointer<Cfhs_TableWidget> m_roiTable;  //算法Roi
    //工位大图
    //key  工位索引
    //value 工位image
    QMap<int, QImage> m_mapStationImg;
    //工位缺陷名
    //key 工位索引
    //value ,map：key 缺陷序号，value 缺陷名
    QMap<int, QMap<int, QString>> m_mapStationDefectName;
    //工位缺陷小图
    //key 工位索引
    //value ,map：key 缺陷序号，value 缺陷图
    QMap<int, QMap<int, QImage>> m_mapStationDefectImg;
    //工位缺陷坐标
    QMap<int, QPolygon> m_mapStationPoint;
    //结果大图
    QImage m_resultImg;
    //结果缺陷名
    //key 缺陷序号
    //value 缺陷名
    QMap<int, QString> m_mapResultDefectName;
    //结果缺陷小图
    //key 缺陷序号
    //value 缺陷图
    QMap<int, QImage> m_mapResultDefectImg;
    //结果缺陷坐标
    QPolygon m_polygonResultPoint;
    //分析大图
    QImage m_analysisImg;
    //分析缺陷坐标
    QPolygon m_polygonAnalysisPoint;
    //九宫格参数
    bool m_openGridview;
    int m_xGridview;
    int m_yGridview;
    //工位特征表
    QMap<int, QString> m_mapStationFeature;
    int m_sumAnalysisTime; //汇总分析缺陷点的时间段
    //配置控制项
    QPointer<QMenu> m_controlMenu;  //控制项Menu
    QPointer<QAction> m_diskMonitorAction; //硬盘监控
    QPointer<QAction> m_softMaintainAction; //软件维护
    QPointer<QAction> m_yieldWarningAction;  //良率预警
    //配置设置项
    QPointer<QMenu> m_settingMenu;  //设置项Menu
    QPointer<QAction> m_programConfigAction; //方案配置
    QPointer<QAction> m_imageSpliceAction;  //图像拼接管理
    QPointer<QAction> m_signalNgAction;  // NG/OK信号管理
    Cfhs_ProgramConfig *m_programConfigWidget;  //方案配置窗口
    QPointer<QAction> m_addCameraAction; //添加相机记录
    //配置帮助项
    QPointer<QMenu> m_helpMenu;
    QPointer<QAction> m_softVersionAction; //软件版本
    QPointer<QAction> m_faultManualAction; //常用问题sop
    QPointer<QAction> m_softManualAction; //软件使用说明书
    QPointer<QAction> m_listConsumablesAction; //易损件清单
    QPointer<QAction> m_teamviewerAction;  //远程协助
    QPointer<QAction> m_defectCallout;  //缺陷打标
    //配置功能项
    QPointer<QMenu> m_functionMenu;
    QPointer<QAction> m_imageManageAction;  //图像显示管理
    QPointer<QAction> m_imageSaveAction;  //图像保存功能
    QPointer<QAction> m_imageSubsetAction;  //图像分区设置
    QPointer<ImgSubSet> m_imgPartitionWidget;  //图像分区界面

};

#endif // CFHS_MAINWINDOW_H
