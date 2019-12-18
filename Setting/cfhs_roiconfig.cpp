#include "cfhs_roiconfig.h"
#include "../cfhs_base.h"
#include "../cfhs_global.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMessageBox>


Cfhs_RoiConfig::Cfhs_RoiConfig(QObject *parent)
    : QObject(parent)
{
    m_shape.Init();
}

Cfhs_RoiConfig::~Cfhs_RoiConfig()
{

}

QString Cfhs_RoiConfig::getParaConfig()
{
    if(m_shape.Points.isEmpty() || m_shape.Shape == 0)
        return "null";

    QJsonObject obj;
    //插入图形索引
    obj.insert("Shape", QString::number(m_shape.Shape));
    //插入坐标列表
    QList<QPointF> list = m_shape.Points;
    QJsonArray dataArray;
    foreach(QPointF point, list)
    {
        //单个坐标点
        QJsonObject pointObj;
        pointObj.insert("X", QString::number(point.x()));
        pointObj.insert("Y", QString::number(point.y()));
        dataArray.append(pointObj);
    }
    obj.insert("Points", dataArray);
    QJsonDocument doc;
    doc.setObject(obj);

    QString strConfig = QString(doc.toJson(QJsonDocument::Compact));

    return strConfig;
}

void Cfhs_RoiConfig::setPointData(const RoiShape &data)
{
    m_shape.Shape = data.Shape;
    m_shape.Points = data.Points;
}

const RoiShape Cfhs_RoiConfig::getPointData()
{
    return m_shape;
}

QString Cfhs_RoiConfig::getShowName()
{
    QString name = tr("手动分区");

    return name;
}

QString Cfhs_RoiConfig::getToolTip()
{
    QString tip = QString(tr("手动绘制ROI进行分区"));
    return tip;
}

QString Cfhs_RoiConfig::getIconPath()
{
    QString path = ":/image_tool/roi_config.png";

    return path;
}

QString Cfhs_RoiConfig::getToolPosition()
{
    return "2-4";
}

bool Cfhs_RoiConfig::setParaConfig(const QString &strConfig)
{
    m_shape.Init();
    if(strConfig.isEmpty() || strConfig == "null")
        return true;
    QByteArray json;
    json.append(strConfig);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    QString strInfo;
    if(error.error != QJsonParseError::NoError)
    {
        strInfo = QString(tr("数据解析失败：%1")).arg(error.errorString());
        QMessageBox::warning(nullptr, " ", strInfo);
        return false;
    }
    if(!doc.isObject())
    {
        strInfo = QString(tr("数据解析失败：%1")).arg("data is not a object");
        QMessageBox::warning(nullptr, " ", strInfo);
        return false;
    }
    QJsonObject obj = doc.object();
    if(!obj.contains("Shape"))
    {
        strInfo = QString(tr("数据解析失败：%1")).arg("data has not Shape");
        QMessageBox::warning(nullptr, " ", strInfo);
        return false;
    }
    int shape = obj.take("Shape").toString().toInt();
    if(!obj.contains("Points"))
    {
        strInfo = QString(tr("数据解析失败：%1")).arg("data has not Points");
        QMessageBox::warning(nullptr, " ", strInfo);
        return false;
    }
    QJsonArray dataArray = obj.take("Points").toArray();
    QList<QPointF> list;
    for(int j=0; j<dataArray.count(); j++)
    {
        QJsonObject dataObj = dataArray.at(j).toObject();
        double x = dataObj.value("X").toString().toDouble();
        double y = dataObj.value("Y").toString().toDouble();
        QPointF point;
        point.setX(x);
        point.setY(y);
        list.append(point);
    }

    m_shape.Shape = shape;
    m_shape.Points = list;

    return true;
}
