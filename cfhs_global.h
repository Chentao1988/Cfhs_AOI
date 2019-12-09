#ifndef CFHS_GLOBAL_H
#define CFHS_GLOBAL_H

#include <QObject>
#include <QPolygon>
#include <QMap>
#include <QStringList>
#include "cfhs_base.h"
#include "Lib/Cfhs_IBusiness.h"

//逻辑接口
extern Cfhs_IBusiness* m_logicInterface;
//当前方案名
extern QString m_curProgramName;
//当前界面语言
extern LanguageEnum m_currentLang;
//当前工位
extern int m_curStationNo;


//程序睡眠
//ms  睡眠时间(毫秒)
extern void Sleep(int ms);

//QString转换成QStringList
extern QStringList getListFromQString(const QString &strValue, const QString& split = "#");

//QString 转换成QPolygon
extern QPolygon getPolygonFromQString(const QString &strValue,
                                      const QString &outSplit = "#",
                                      const QString &inSplit = ",");
//获取Json的key列表
//json  Json格式的QString
//list  转换为的list
//strInfo  操作日志
extern bool getKeyListFromJson(const QString &json, QStringList &list, QString &strInfo);
//获取Json的value列表
//json  Json格式的QString
//list  转换为的list
//strInfo  操作日志
extern bool getValueListFromJson(const QString &json, QStringList &list, QString &strInfo);
//Json转换为Map
//json  Json格式的QString
//map  转换为的map
//strInfo  操作日志
extern bool getMapFromJson(const QString &json, QMap<QString, QString> &map, QString &strInfo);
extern bool getMapFromJson(const QString &json, QMap<QString, int> &map, QString &strInfo);
//Map转换为Json
extern QString getJsonFromMap(const QMap<QString, QString> &map);



#endif // CFHS_GLOBAL_H
