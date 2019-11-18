#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QDateTime>

#if defined(CFHS_BUSINESS_LIBRARY)
#  define CFHS_IBUSINESS_EXPORT Q_DECL_EXPORT
#else
#  define CFHS_IBUSINESS_EXPORT
#endif

#define strStart QObject::tr("开始")
#define strStop QObject::tr("终止")

#define strChinese QObject::tr("中文")
#define strEnglish QObject::tr("英文")

#define strMySQLIP              "192.168.0.180"
#define table_config            "cfhs.table_config"
#define table_programme         "cfhs.table_programme"
#define table_camerainfo        "cfhs.table_camerainfo"
#define table_batch             "cfhs.table_batch"
#define table_batch_img         "cfhs.table_batch_img"
#define table_station           "cfhs.table_station"
#define table_features          "cfhs.table_features"
#define table_station_result    "cfhs.table_station_result"
#define table_final_result      "cfhs.table_final_result"

typedef struct Config
{
    //管理员密码
    QString strPassword;
    //当前方案名
    QString strNowProName;
    //当前批次名
    QString strNowBatchName;
    //控制系统的IP地址
    QString strControlIP;
    //控制系统的端口
    quint16 usControlPort;
    //磁盘空间小于多少G时，报警
    int iDiskSpaceAlarm;
    //磁盘空间小于多少G时，删除
    int iDiskSpaceDelete;
    //保存OK原图片，1-保存，0-不保存
    bool bSaveOKImg;

    bool bSaveNGImg;

    bool bSaveCompressedOKImg;

    bool bSaveCompressedNGImg;

    bool bStationDynamic;

    bool bAlgorithmOnOrOff;
    //产品运行多少片后开始计算
    int iYieldAlarmNum;
    //良率低于多少开始预警
    QString strYieldAlarm;
    //系统语言：2-繁体中文，1-英文，0-简体中文
    int iSysLang;
    //软件离线：1-离线，0-在线
    bool bSysOnLine;
    //导入图片路径
    QString strInImage;
    //下载文档路径
    QString strDownFilePath;

}stConfig;

typedef struct Programme
{
    //方案名
    QString strProName;
    //打开扫码枪,true(开启)，false(关闭)
    bool bOpenScan;
    //扫码总数
    int iScanTotal;
    //扫码成功数
    int iScanPass;
    //扫码率
    QString strScanRate;
    //打开MES上传,true(开启)，false(关闭)
    bool bOpenMes;
    //上传总数
    int iMesTotal;
    //上传数
    int iMesPass;
    //上传率
    QString strMesRate;
    //1-正常结果，2-强制OK，3-强制NG
    int iResultMode;
    //缺陷判定(JSON格式的字符串，例如"{"交叉划痕":{"1":"划痕","2":"划痕"},"交叉脏污":{"1":"脏污","2":"脏污"},"划痕":{"1":"","2":"划痕"},"脏污":{"1":"划痕","2":"脏污"}}")
    QString strDefectDetermination;
    //缺陷筛选(JSON格式的字符串，例如"{"白点": "((缺陷面积1>2)&&(缺陷面积2>0))||(缺陷宽度>=4)", "黑点": "(缺陷面积1>=7)&&(缺陷面积2<8)"}")
    QString strNGScreening;

}stProgramme;

typedef struct CameraInfo
{
    //相机类型
    QString strCameraType;
    //相机规格
    QString strCameraSpecification;
    //ccf文件路径
    QString strCcfFilePath;

}stCameraInfo;

typedef struct Batch
{
    //批次名
    QString strBatchName;
    //检测总数
    int iCheckTotal;
    //OK总数
    int iOKTotal;
    //NG总数
    int iNGTotal;
    //产品良率
    QString strYield;

}stBatch;

typedef struct BatchImg
{
    //批次名
    QString strBatch;
    //产品ID
    QString strProductID;
    //检查结果，OK(良品),NG(不良品)
    QString strResult;
    //插入时间
    QDateTime dtInsertTime;

}stBatchImg;

typedef struct Station
{
    //方案名
    QString strProName;
    //工位号
    int iStationNo;
    //下位机IP地址
    QString strIP;
    //下位机端口
    quint16 usPort;
    //缺陷数量
    int iDefectNum;
    //缺陷定义(JSON格式的字符串，例如"{"白点": "((缺陷面积1>2)&&(缺陷面积2>0))||(缺陷宽度>=4)", "黑点": "(缺陷面积1>=7)&&(缺陷面积2<8)"}")
    QString strDefectDefine;
    //缺陷筛选(JSON格式的字符串，例如"{"白点": "((缺陷面积1>2)&&(缺陷面积2>0))||(缺陷宽度>=4)", "黑点": "(缺陷面积1>=7)&&(缺陷面积2<8)"}")
    QString strDefectScreening;
    //是否启用，true(开启)，false(关闭)
    bool bEnable;
    //工具参数(JSON格式的字符串)
    QString strToolPara;

}stStation;

typedef struct Features
{
    //特征中文名(特征名1#特征名2#...特征名N)
    QString strCH;
    //特征英文号(特征名1#特征名2#...特征名N)
    QString strEN;
    //特征繁体号(特征名1#特征名2#...特征名N)
    QString strTr;

}stFeatures;

//解决中文乱码
#pragma execution_character_set("utf-8")

#endif // GLOBAL_H
