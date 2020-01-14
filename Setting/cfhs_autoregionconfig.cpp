#include "cfhs_autoregionconfig.h"
#include <QJsonDocument>
#include <QJsonObject>


Cfhs_AutoRegionConfig::Cfhs_AutoRegionConfig(QWidget *parent)
    :Cfhs_ToolBase(parent)
{

    //功能名
    setFunctionName(tr("分区参数"));
    //参数表
    insertOnePara("aa_region_value_min", tr("边缘低阈值"), tr("a区提取亮度下限"));
    insertOnePara("aa_region_value_max", tr("边缘高阈值"), tr("a区提取亮度上限"));
    //工具名
    this->setWindowTitle(getShowName());
    //窗口大小
    this->resize(600, 400);
}

Cfhs_AutoRegionConfig::~Cfhs_AutoRegionConfig()
{

}

QString Cfhs_AutoRegionConfig::getShowName()
{
    QString name = tr("自动AA区1");

    return name;
}

QString Cfhs_AutoRegionConfig::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_AutoRegionConfig::getIconPath()
{
    QString path = ":/image_tool/auto_region_config.png";

    return path;
}

QString Cfhs_AutoRegionConfig::getToolPosition()
{
    return "2-1";
}

QString Cfhs_AutoRegionConfig::getToolParaDefault()
{
    QString strPara;
    QJsonObject obj;
    obj.insert("aa_region_value_min", "130");
    obj.insert("aa_region_value_max", "255");
    QJsonDocument doc;
    doc.setObject(obj);

    strPara = QString(doc.toJson(QJsonDocument::Compact));
    return strPara;
}

QString Cfhs_AutoRegionConfig::getParaConfig()
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(m_strConfig, map, strInfo);
    if(map.isEmpty())
        return getToolParaDefault();
    else
        return m_strConfig;
}

bool Cfhs_AutoRegionConfig::setParaConfig(const QString &strConfig)
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(strConfig, map, strInfo);
    if(map.isEmpty())
    {
        //数据错误，使用默认数据
        QString strDefault = getToolParaDefault();
        getMapFromJson(strDefault, map, strInfo);
        m_strConfig = strDefault;
    }
    int row = 0;
    foreach(ParaInfo info, m_vectorPara)
    {
        if(map.contains(info.m_toolName))
        {
            QString strValue = map.value(info.m_toolName);
            row = info.m_index - 1;
            QTableWidgetItem *item = m_algoTable->item(row, 1);
            if(!item)
            {
                item = new QTableWidgetItem;
                m_algoTable->setItem(row, 1, item);
            }
            item->setText(strValue);
        }
    }

    return true;
}
