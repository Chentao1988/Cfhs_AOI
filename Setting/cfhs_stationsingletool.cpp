#include "cfhs_stationsingletool.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


Cfhs_StationSingleTool::Cfhs_StationSingleTool(const StationTool &tool, QListWidget *parent)
    : QListWidgetItem (parent)
{
    m_stationTool = tool;
    //设置工具名
    setToolPosition(m_stationTool.m_toolPosition);
}

Cfhs_StationSingleTool::~Cfhs_StationSingleTool()
{

}

StationTool Cfhs_StationSingleTool::getStationTool()
{
    return m_stationTool;
}

QString Cfhs_StationSingleTool::getIconPath(const QString &toolPosition)
{
    QString iconPath = "";
    if(toolPosition == Cfhs_CameraConfig::getToolPosition())
        iconPath = Cfhs_CameraConfig::getIconPath();
    else if(toolPosition == Cfhs_AutoRegionConfig::getToolPosition())
        iconPath = Cfhs_AutoRegionConfig::getIconPath();
    else if(toolPosition == Cfhs_DefectConfig::getToolPosition())
        iconPath = Cfhs_DefectConfig::getIconPath();
    else if(toolPosition == Cfhs_AutoRegionConfig_hjh::getToolPosition())
        iconPath = Cfhs_AutoRegionConfig_hjh::getIconPath();
    else if(toolPosition == Cfhs_DefectConfig_hjh::getToolPosition())
        iconPath = Cfhs_DefectConfig_hjh::getIconPath();
    else if(toolPosition == Cfhs_OriginalRegionConfig::getToolPosition())
        iconPath = Cfhs_OriginalRegionConfig::getIconPath();
    else if(toolPosition == Cfhs_ItoDetectConfig::getToolPosition())
        iconPath = Cfhs_ItoDetectConfig::getIconPath();

    return iconPath;
}

QString Cfhs_StationSingleTool::getToolTip(const QString &toolPosition)
{
    QString tip = "";
    if(toolPosition == Cfhs_CameraConfig::getToolPosition())
        tip = Cfhs_CameraConfig::getToolTip();
    else if(toolPosition == Cfhs_AutoRegionConfig::getToolPosition())
        tip = Cfhs_AutoRegionConfig::getToolTip();
    else if(toolPosition == Cfhs_DefectConfig::getToolPosition())
        tip = Cfhs_DefectConfig::getToolTip();
    else if(toolPosition == Cfhs_AutoRegionConfig_hjh::getToolPosition())
        tip = Cfhs_AutoRegionConfig_hjh::getToolTip();
    else if(toolPosition == Cfhs_DefectConfig_hjh::getToolPosition())
        tip = Cfhs_DefectConfig_hjh::getToolTip();
    else if(toolPosition == Cfhs_OriginalRegionConfig::getToolPosition())
        tip = Cfhs_OriginalRegionConfig::getToolTip();
    else if(toolPosition == Cfhs_ItoDetectConfig::getToolPosition())
        tip = Cfhs_ItoDetectConfig::getToolTip();

    return tip;
}

QString Cfhs_StationSingleTool::getShowName(const QString &toolPosition)
{
    QString showName = "";
    if(toolPosition == Cfhs_CameraConfig::getToolPosition())
        showName = Cfhs_CameraConfig::getShowName();
    else if(toolPosition == Cfhs_AutoRegionConfig::getToolPosition())
        showName = Cfhs_AutoRegionConfig::getShowName();
    else if(toolPosition == Cfhs_DefectConfig::getToolPosition())
        showName = Cfhs_DefectConfig::getShowName();
    else if(toolPosition == Cfhs_AutoRegionConfig_hjh::getToolPosition())
        showName = Cfhs_AutoRegionConfig_hjh::getShowName();
    else if(toolPosition == Cfhs_DefectConfig_hjh::getToolPosition())
        showName = Cfhs_DefectConfig_hjh::getShowName();
    else if(toolPosition == Cfhs_OriginalRegionConfig::getToolPosition())
        showName = Cfhs_OriginalRegionConfig::getShowName();
    else if(toolPosition == Cfhs_ItoDetectConfig::getToolPosition())
        showName = Cfhs_ItoDetectConfig::getShowName();


    return showName;
}

QString Cfhs_StationSingleTool::getToolParaDefault(const QString &toolPosition)
{
    QString toolPara = "null";
    if(toolPosition == Cfhs_AutoRegionConfig::getToolPosition())
        toolPara = Cfhs_AutoRegionConfig::getToolParaDefault();
    else if(toolPosition == Cfhs_DefectConfig::getToolPosition())
        toolPara = Cfhs_DefectConfig::getToolParaDefault();
    else if(toolPosition == Cfhs_AutoRegionConfig_hjh::getToolPosition())
        toolPara = Cfhs_AutoRegionConfig_hjh::getToolParaDefault();
    else if(toolPosition == Cfhs_DefectConfig_hjh::getToolPosition())
        toolPara = Cfhs_DefectConfig_hjh::getToolParaDefault();
    else if(toolPosition == Cfhs_ItoDetectConfig::getToolPosition())
        toolPara = Cfhs_ItoDetectConfig::getToolParaDefault();

    return toolPara;
}

void Cfhs_StationSingleTool::setShowName(const QString &name)
{
    m_stationTool.m_showName = name;
    QString showName = QString("%1.%2").arg(m_stationTool.m_index)
            .arg(name);
    this->setText(showName);
}

void Cfhs_StationSingleTool::setToolIndex(const int &index)
{
    m_stationTool.m_index = index;
    QString showName = QString("%1.%2").arg(m_stationTool.m_index)
            .arg(m_stationTool.m_showName);
    this->setText(showName);
}

void Cfhs_StationSingleTool::setToolPara(const QString &para)
{
    m_stationTool.m_toolPara = para;
}

void Cfhs_StationSingleTool::setToolPosition(const QString& strPosition)
{
    m_stationTool.m_toolPosition = strPosition;
    QString showName = getShowName(strPosition);
    QString tip = getToolTip(strPosition);
    QString iconPath = getIconPath(strPosition);
    if(m_stationTool.m_times > 0)
        showName = QString("%1_%2").arg(showName).arg(m_stationTool.m_times);

    setShowName(showName);
    this->setToolTip(tip);
    QIcon icon;
    icon.addFile(iconPath);
    this->setIcon(icon);
    if(m_stationTool.m_toolPara.isEmpty()
            || m_stationTool.m_toolPara == "null")
    {
        //设置默认参数
        setToolPara(getToolParaDefault(strPosition));
    }
}
