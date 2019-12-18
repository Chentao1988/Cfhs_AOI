#ifndef CFHS_IBUSINESS_H
#define CFHS_IBUSINESS_H

#include "Cfhs_Global.h"

class QThread;
class Cfhs_Manual;
class Cfhs_ThreadMain;
class Cfhs_ThreadRefresh;
class Cfhs_ThreadOffLine;

class CFHS_IBUSINESS_EXPORT Cfhs_IBusiness : public QObject
{
    Q_OBJECT

public:

    Cfhs_IBusiness();

    ~Cfhs_IBusiness();

    //初始化逻辑接口
    //输出
    //strInfo:执行信息
    bool Init(QString &strInfo);
    //退出逻辑接口
    //输出
    //strInfo:执行信息
    void Exit();

    //格式检查
    bool FormatCheck(const QString &strNowName, int iStation, QString strCheck, QString &strInfo);

    //***登录界面
public:
    //---登录
    //输入
    //strPWD:管理员密码
    //输出
    //strInfo:执行信息
    bool Login(const QString &strPWD, QString &strInfo);
    //---修改密码
    //输入
    //strNowPWD:当前密码
    //strNewPWD:新密码
    //输出
    //strInfo:执行信息
    bool EditPWD(const QString &strNowPWD, const QString &strNewPWD, QString &strInfo);

    //***主界面
public:
    //---获取配置信息信息
    //输出
    //stGet:配置信息
    //strInfo:执行信息
    bool GetConfigInfo(stConfig &stGet, QString &strInfo);
    //---设置配置信息信息
    //输入
    //stGet:配置信息
    //输出
    //strInfo:执行信息
    bool SetConfigInfo(const stConfig &stSet, QString &strInfo);

    //***table_pro表操作
    //---获取所有方案名
    //输出
    //strAllName:系统存在的所有方案名(方案名1#方案名2#...方案名N)
    //strInfo:执行信息
    bool GetAllProName(QString &strAllName, QString &strInfo);
    //---插入方案名
    //输入
    //strNewName：新方案名
    //输出
    //strInfo:执行信息
    bool InsertNewProName(const QString &strNewName, QString &strInfo);
    //---删除方案名
    //输入
    //strName：方案名
    //输出
    //strInfo:执行信息
    bool DeleteProName(const QString &strName, QString &strInfo);
    //---获取方案信息
    //输入
    //strNowName：系统的当前方案名
    //stGet:输出
    //bShowImg:是否显示图片
    //方案信息
    //strInfo:执行信息
    bool GetProInfo(const QString &strNowName, stProgramme &stGet, bool bShowImg, QString &strInfo);
    //---设置方案信息
    //输入
    //strNowName：系统的当前方案名
    //stSet:方案信息
    //strInfo:执行信息
    bool SetProInfo(const QString &strNowName,const stProgramme &stSet, QString &strInfo);

    //***table_batch表操作
    //---获取所有批次名
    //输出
    //strAllName:系统存在的所有批次名(批次1#批次2#...批次N)
    //strInfo:执行信息
    bool GetAllBatchName(QString &strAllName, QString &strInfo);
    //---插入新批次名
    //输入
    //strNewName：新批次名
    //输出
    //strInfo:执行信息
    bool InsertNewBatchName(const QString &strNewName, QString &strInfo);
    //---获取批次信息
    //输入
    //strNowName：系统的当前批次名
    //输出
    //stGet:批次信息
    //strInfo:执行信息
    bool GetBatchInfo(const QString &strNowName, stBatch &stGet, QString &strInfo);
    //---设置批次信息
    //输入
    //strNowName：系统的当前批次名
    //stSet:批次信息
    //输出
    //strInfo:执行信息
    bool SetBatchInfo(const QString &strNowName, const stBatch &stSet, QString &strInfo);

    //***table_station表操作
    //---获取所有工位名
    //输入
    //strNowName：系统的当前批次名
    //输出
    //strAllNo:系统存在的所有工位号(工位1#工位2#工位3#...工位N)
    //strInfo:执行信息
    bool GetAllStationNo(const QString &strNowName, QString &strAllNo, QString &strInfo);
    //---插入工位号
    //输入
    //strProName：方案名
    //iStationNo：工位号
    //输出
    //strInfo:执行信息
    bool InsertNewStationNo(const QString &strProName, int iStationNo, QString &strInfo);
    //---删除工位号
    //输入
    //strProName：方案名
    //iStationNo：工位号
    //输出
    //strInfo:执行信息
    bool DeleteStationNo(const QString &strProName, int iStationNo, QString &strInfo);
    //---获取工位信息
    //输入
    //strProName：方案名
    //iStationNo：工位号
    //输出
    //stGet:工位信息
    //strInfo:执行信息
    bool GetStationInfo(const QString &strProName, int iStationNo, stStation &stGet, QString &strInfo);
    //---设置工位信息
    //输入
    //stSet:工位信息
    //输出
    //strInfo:执行信息
    bool SetStationInfo(const QString &strProName, int iStationNo, const stStation &stSet, QString &strInfo);

    //***table_camerainfo
    //strCameraInfo:相机信息（取值格式：相机类型1#相机规格1#ccf文件路径1@相机类型N#相机规格N#ccf文件路径N)
    //strInfo:执行信息
    bool GetCameraInfo(QString &strCameraInfo, QString &strInfo);

    //stCamera:相机信息
    //strInfo:执行信息
    bool InsertCameraInfo(stCameraInfo stCamera, QString &strInfo);

    //---获取指定时间的分析信息
    //iHour:前几小时
    //strPoint:需显示在大图上的缺陷点（取值格式：X坐标1,Y坐标1#X坐标2,Y坐标2#...X坐标N,Y坐标N）
    //strInfo:执行信息
    bool GetAnalysisByHour(int iHour, QString &strPoint, QString &strInfo);

    //---开始
    void Start();
    //---停止
    void Stop();
    //---关闭所有软件和电脑
    void CloseAll();

    //---离线模式
    void OffLineInit();
    //---退出离线模式
    void OffLineExit();
    //---导入图片
    void OffLineInImg(const QString &strPath);
    //---导出图片
    void OffLineOutImg();
    //---复测执行
    void OffLineOne();
    //---上一张
    void OffLineLast();
    //---下一张
    void OffLineNext();
    //---循环模式
    void OffLineLoop();
    //---查找产品ID
    void OffLineCheckID();
    //---下载文件区域
    void OffLineDownFile(const QString &strPath);
    //---报表查询导出
    void OffLineReport();

signals:

    //显示软件状态
    //bRun:软件是否正常运行（true：运行中；false：已停止）
    void sig_ShowSoftStatus(bool bRun);

    //---显示弹框信息(离线模式)
    //bNormal:true-正常信息，正常字体;false-异常信息，红色字体
    //strInfo:弹框信息
    void sig_ShowFrameInfo(bool bNormal, const QString &strInfo);

    //---显示日志信息
    //bNormal:true-正常信息，白色字体;false-异常信息，红色字体
    //strInfo:执行信息
    void sig_ShowInfo(bool bNormal,const QString &strInfo);

    //---显示扫码率
    //输入
    //strInfo:扫码率
    void sig_ShowScanRate(const QString &strInfo);

    //---显示Mes上报率
    //输入
    //strInfo:上报率
    void sig_ShowMesRate(const QString &strInfo);

    //---显示特征信息
    //输入
    //iStationNo:工位号
    //strData:特征信息（取值格式：产品ID#缺陷ID#缺陷名称#特征1#特征2...#特征N@产品ID#缺陷ID#缺陷名称#特征1#特征2...#特征N）
    void sig_ShowFeatureData(int iStationNo, const QString &strData);

    //---显示工位连接状态
    //输入
    //iStationNo:工位号
    //iConnState:连接状态(取值：1代表已连接,2代表未连接,3代表待机)
    void sig_ShowStationStatus(int iStationNo, int iConnState);

    //---显示当前产品结果
    //输入
    //strResult:结果信息(取值：OK代表良品,NG代表不良品)
    void sig_ShowNowResult(const QString &strResult);

    //---显示前20个的结果
    //输入
    //strResult:结果信息（取值格式：第1个结果#第2个结果...#第20个结果）(取值：OK代表良品,NG代表不良品)
    void sig_Show20Result(const QString &strResult);

    //---显示批次表单信息
    //输入
    //BatchInfo:批次表单信息（取值格式：检测批次#检测总量#合格数量#次品数量#产品良率）
    void sig_ShowBatchFormInfo(const QString &BatchInfo);

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
    void sig_ShowStationImagePoint(int iStation, const QImage &imageBig, const QString &strDefectName1, const QString &strDefectName2, const QString &strDefectName3, const QImage &imageSmall1, const QImage &imageSmall2, const QImage &imageSmall3, const QString &strPoint);

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
    void sig_ShowResultImagePoint(const QImage &imageBig, const QString &strDefectName1, const QString &strDefectName2, const QString &strDefectName3, const QImage &imageSmall1, const QImage &imageSmall2, const QImage &imageSmall3, const QString &strPoint);

    //---显示分析图片和缺陷点
    //输入
    //imageBig:需显示的大图
    //strPoint:需显示在大图上的缺陷点（取值格式：X坐标1,Y坐标1#X坐标2,Y坐标2#...X坐标N,Y坐标N）
    void sig_ShowAnalysisImagePoint(const QImage &imageBig, const QString &strPoint);

    //---显示批次图表信息
    //输入
    //strHourName:小时名字（取值格式：小时名1#小时名2...#小时名12）
    //strHourInput:每小时投入（取值格式：小时投入1#小时投入2...#小时投入12）
    //strHourYield:每小时良率（取值格式：小时良率1#小时良率2...#小时良率12）
    //strHourTotalYield:每小时累积良率（取值格式：小时累积良率1#小时累积良率2...#小时累积良率12）
    void sig_ShowBatchChartInfo(const QString &strHourName, const QString &strHourInput, const QString &strHourYield, const QString &strHourTotalYield);

    //---磁盘监控报警
    //strInfo:信息
    void sig_ShowDiskSpaceAlarm(const QString &strInfo);

    //---良率报警
    //strInfo:信息
    void sig_ShowYieldAlarm(const QString &strInfo);

    void sig_Init();

    void sig_Exit();

    void sig_InImg(const QString &strPath);

    void sig_OutImg();

    void sig_One();

    void sig_Last();

    void sig_Next();

    void sig_Loop();

    void sig_CheckID();

    void sig_DownFile(const QString &strPath);

    void sig_Report();

private:

    QThread             *m_pRefreshThread;
    QThread             *m_pOffLineThread;
    Cfhs_Manual         *m_pCfhs_Manual;
    Cfhs_ThreadMain     *m_pCfhs_ThreadMain;
    Cfhs_ThreadRefresh  *m_pCfhs_ThreadRefresh;
    Cfhs_ThreadOffLine  *m_pCfhs_ThreadOffLine;

};
#endif // CFHS_IBUSINESS_H
