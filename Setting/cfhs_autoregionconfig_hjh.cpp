#include "cfhs_autoregionconfig_hjh.h"
#include <QJsonDocument>
#include <QJsonObject>

Cfhs_AutoRegionConfig_hjh::Cfhs_AutoRegionConfig_hjh(QWidget *parent)
    :Cfhs_ToolBase(parent)
{
    //功能名
    setFunctionName(tr("分区参数"));
    //参数表
    insertOnePara("aa_region_negative", tr("A区低阈值"), tr("相对于A区平均亮度下沉值"));
    insertOnePara("aa_region_positive", tr("A区高阈值"), tr("相对于A区平均亮度上浮值"));
    insertOnePara("new_model", tr("模板"), tr("0：调用已有模板  1：新建模板"));
    //工具名
    this->setWindowTitle(getShowName());
    //窗口大小
    this->resize(600, 400);
}

Cfhs_AutoRegionConfig_hjh::~Cfhs_AutoRegionConfig_hjh()
{

}

QString Cfhs_AutoRegionConfig_hjh::getShowName()
{
    QString name = tr("自动分区2");

    return name;
}

QString Cfhs_AutoRegionConfig_hjh::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_AutoRegionConfig_hjh::getIconPath()
{
    QString path = ":/image_tool/auto_region_config.png";

    return path;
}

QString Cfhs_AutoRegionConfig_hjh::getToolPosition()
{
    return "2-2";
}

QString Cfhs_AutoRegionConfig_hjh::getToolParaDefault()
{
    QString strPara;
    QJsonObject obj;
    obj.insert("aa_region_negative", "30");
    obj.insert("aa_region_positive", "30");
    obj.insert("new_model", "1");
    QJsonDocument doc;
    doc.setObject(obj);

    strPara = QString(doc.toJson(QJsonDocument::Compact));
    return strPara;
}

QString Cfhs_AutoRegionConfig_hjh::getParaConfig()
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(m_strConfig, map, strInfo);
    if(map.isEmpty())
        return getToolParaDefault();
    else
        return m_strConfig;
}

bool Cfhs_AutoRegionConfig_hjh::setParaConfig(const QString &strConfig)
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
