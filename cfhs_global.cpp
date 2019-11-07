#include "cfhs_global.h"
#include <QTime>
#include <QPoint>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>


Cfhs_IBusiness *m_logicInterface = new Cfhs_IBusiness();
QString m_curProgramName = "";
LanguageEnum m_currentLang = SimplifiedChinese;


void Sleep(int ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < ms)
        QCoreApplication::processEvents();
}

QStringList getListFromQString(const QString &strValue, const QString &split)
{
    if(strValue.isEmpty())
        return QStringList();

    QStringList list = strValue.split(split);

    return list;
}

QPolygon getPolygonFromQString(const QString &strValue,
                               const QString &outSplit,
                               const QString &inSplit)
{
    QPolygon polygon;
    QStringList list = strValue.split(outSplit);
    for(int i=0; i<list.size(); i++)
    {
        QPoint point;
        int x = list.at(i).section(inSplit, 0, 0).toInt();
        int y = list.at(i).section(inSplit, 1).toInt();
        point.setX(x);
        point.setY(y);
        polygon.append(point);
    }

    return polygon;
}

bool getKeyListFromJson(const QString &json, QStringList &list, QString &strInfo)
{
    //清空原有数据
    list.clear();
    if(json.isEmpty() || json == "null")
    {
        strInfo = "Conversion successed";
        return true;
    }
    QJsonParseError error;
    QByteArray info;
    info.append(json);
    QJsonDocument doc = QJsonDocument::fromJson(info, &error);
    if(error.error != QJsonParseError::NoError)
    {
        strInfo = QString("Data parsing failed: %1").arg(error.errorString());
        return false;
    }
    if(!doc.isObject())
    {
        strInfo = QString("Data parsing failed: %1").arg("not a object");
        return false;
    }
    QJsonObject obj = doc.object();
    QJsonObject::const_iterator iter = obj.begin();
    while(iter != obj.end())
    {
        list.append(iter.key());
        iter++;
    }

    strInfo = "Conversion successed";
    return true;
}



bool getValueListFromJson(const QString &json, QStringList &list, QString &strInfo)
{
    //清空原有数据
    list.clear();
    if(json.isEmpty() || json == "null")
    {
        strInfo = "Conversion successed";
        return true;
    }
    QJsonParseError error;
    QByteArray info;
    info.append(json);
    QJsonDocument doc = QJsonDocument::fromJson(info, &error);
    if(error.error != QJsonParseError::NoError)
    {
        strInfo = QString("Data parsing failed: %1").arg(error.errorString());
        return false;
    }
    if(!doc.isObject())
    {
        strInfo = QString("Data parsing failed: %1").arg("not a object");
        return false;
    }
    QJsonObject obj = doc.object();
    QJsonObject::const_iterator iter = obj.begin();
    while(iter != obj.end())
    {
        list.append(iter.value().toString());
        iter++;
    }

    strInfo = "Conversion successed";
    return true;
}

bool getMapFromJson(const QString &json, QMap<QString, QString> &map, QString &strInfo)
{
    //清空原有数据
    map.clear();
    if(json.isEmpty() || json == "null")
    {
        strInfo = "Conversion successed";
        return true;
    }
    QJsonParseError error;
    QByteArray info;
    info.append(json);
    QJsonDocument doc = QJsonDocument::fromJson(info, &error);
    if(error.error != QJsonParseError::NoError)
    {
        strInfo = QString("Data parsing failed: %1").arg(error.errorString());
        return false;
    }
    if(!doc.isObject())
    {
        strInfo = QString("Data parsing failed: %1").arg("not a object");
        return false;
    }
    QJsonObject obj = doc.object();
    QJsonObject::const_iterator iter = obj.begin();
    while(iter != obj.end())
    {
        map.insert(iter.key(), iter.value().toString());
        iter++;
    }

    strInfo = "Conversion successed";
    return true;
}

QString getJsonFromMap(const QMap<QString, QString> &map)
{
    if(map.isEmpty())
        return "null";
    QMap<QString, QString>::const_iterator iter = map.begin();
    QJsonObject obj;
    while(iter != map.end())
    {
        obj.insert(iter.key(), iter.value());
        iter++;
    }
    QJsonDocument doc;
    doc.setObject(obj);
    QString json = QString(doc.toJson(QJsonDocument::Compact));

    return json;
}

bool getMapFromJson(const QString &json, QMap<QString, int> &map, QString &strInfo)
{
    //清空原有数据
    map.clear();
    if(json.isEmpty() || json == "null")
    {
        strInfo = "Conversion successed";
        return true;
    }
    QJsonParseError error;
    QByteArray info;
    info.append(json);
    QJsonDocument doc = QJsonDocument::fromJson(info, &error);
    if(error.error != QJsonParseError::NoError)
    {
        strInfo = QString("Data parsing failed: %1").arg(error.errorString());
        return false;
    }
    if(!doc.isObject())
    {
        strInfo = QString("Data parsing failed: %1").arg("not a object");
        return false;
    }
    QJsonObject obj = doc.object();
    QJsonObject::const_iterator iter = obj.begin();
    while(iter != obj.end())
    {
        map.insert(iter.key(), iter.value().toInt());
        iter++;
    }

    strInfo = "Conversion successed";
    return true;
}
