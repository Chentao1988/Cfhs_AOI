#include "cfhs_lightfenceconfig.h"


Cfhs_LightFenceConfig::Cfhs_LightFenceConfig(QWidget *parent)
    : Cfhs_ToolBase(parent)
{

}

Cfhs_LightFenceConfig::~Cfhs_LightFenceConfig()
{

}

QString Cfhs_LightFenceConfig::getShowName()
{
    QString name = tr("光栅检测");

    return name;
}

QString Cfhs_LightFenceConfig::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_LightFenceConfig::getIconPath()
{
    QString path = ":/image_tool/light_fence_config.png";

    return path;
}

QString Cfhs_LightFenceConfig::getToolPosition()
{
    return "3-4";
}

QString Cfhs_LightFenceConfig::getToolParaDefault()
{
    QString strPara;

    return strPara;
}

QString Cfhs_LightFenceConfig::getParaConfig()
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(m_strConfig, map, strInfo);
    if(map.isEmpty())
        return getToolParaDefault();
    else
        return m_strConfig;
}

bool Cfhs_LightFenceConfig::setParaConfig(const QString &strConfig)
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
