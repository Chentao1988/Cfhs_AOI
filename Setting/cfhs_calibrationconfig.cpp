#include "cfhs_calibrationconfig.h"


Cfhs_CalibrationConfig::Cfhs_CalibrationConfig(QWidget *parent)
    : Cfhs_ToolBase(parent)
{

}

Cfhs_CalibrationConfig::~Cfhs_CalibrationConfig()
{

}

QString Cfhs_CalibrationConfig::getShowName()
{
    QString name = tr("相机标定");

    return name;
}

QString Cfhs_CalibrationConfig::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_CalibrationConfig::getIconPath()
{
    QString path = ":/image_tool/calibration_config.png";

    return path;
}

QString Cfhs_CalibrationConfig::getToolPosition()
{
    return "3-5";
}

QString Cfhs_CalibrationConfig::getToolParaDefault()
{
    QString strPara;

    return strPara;
}

QString Cfhs_CalibrationConfig::getParaConfig()
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(m_strConfig, map, strInfo);
    if(map.isEmpty())
        return getToolParaDefault();
    else
        return m_strConfig;
}

bool Cfhs_CalibrationConfig::setParaConfig(const QString &strConfig)
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
