#include "cfhs_stationsingletool.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


Cfhs_StationSingleTool::Cfhs_StationSingleTool(const StationTool &tool, QListWidget *parent)
    : QListWidgetItem (parent)
{
    m_stationTool = tool;
    //设置工具名
    setToolName(m_stationTool.m_toolName);
}

Cfhs_StationSingleTool::~Cfhs_StationSingleTool()
{

}

StationTool Cfhs_StationSingleTool::getStationTool()
{
    return m_stationTool;
}

QString Cfhs_StationSingleTool::getIconPath(const QString &toolName)
{
    QString iconPath = "";
    if(toolName == Cfhs_CameraConfig::getToolName())
        iconPath = Cfhs_CameraConfig::getIconPath();
    else if(toolName == Cfhs_RoiConfig::getToolName())
        iconPath = Cfhs_RoiConfig::getIconPath();
    else if(toolName == Cfhs_AutoRegionConfig::getToolName())
        iconPath = Cfhs_AutoRegionConfig::getIconPath();
    else if(toolName == Cfhs_DefectConfig::getToolName())
        iconPath = Cfhs_DefectConfig::getIconPath();
    else if(toolName == Cfhs_WaveFilterConfig::getToolName())
        iconPath = Cfhs_WaveFilterConfig::getIconPath();
    else if(toolName == Cfhs_AutoRegionConfig_hjh::getToolName())
        iconPath = Cfhs_AutoRegionConfig_hjh::getIconPath();
    else if(toolName == Cfhs_DefectConfig_hjh::getToolName())
        iconPath = Cfhs_DefectConfig_hjh::getIconPath();

    return iconPath;
}

QString Cfhs_StationSingleTool::getToolTip(const QString &toolName)
{
    QString tip = "";
    if(toolName == Cfhs_CameraConfig::getToolName())
        tip = Cfhs_CameraConfig::getToolTip();
    else if(toolName == Cfhs_RoiConfig::getToolName())
        tip = Cfhs_RoiConfig::getToolTip();
    else if(toolName == Cfhs_AutoRegionConfig::getToolName())
        tip = Cfhs_AutoRegionConfig::getToolTip();
    else if(toolName == Cfhs_DefectConfig::getToolName())
        tip = Cfhs_DefectConfig::getToolTip();
    else if(toolName == Cfhs_WaveFilterConfig::getToolName())
        tip = Cfhs_WaveFilterConfig::getToolTip();
    else if(toolName == Cfhs_AutoRegionConfig_hjh::getToolName())
        tip = Cfhs_AutoRegionConfig_hjh::getToolTip();
    else if(toolName == Cfhs_DefectConfig_hjh::getToolName())
        tip = Cfhs_DefectConfig_hjh::getToolTip();

    return tip;
}

QString Cfhs_StationSingleTool::getShowName(const QString &toolName)
{
    QString showName = "";
    if(toolName == Cfhs_CameraConfig::getToolName())
        showName = Cfhs_CameraConfig::getShowName();
    else if(toolName == Cfhs_RoiConfig::getToolName())
        showName = Cfhs_RoiConfig::getShowName();
    else if(toolName == Cfhs_AutoRegionConfig::getToolName())
        showName = Cfhs_AutoRegionConfig::getShowName();
    else if(toolName == Cfhs_DefectConfig::getToolName())
        showName = Cfhs_DefectConfig::getShowName();
    else if(toolName == Cfhs_WaveFilterConfig::getToolName())
        showName = Cfhs_WaveFilterConfig::getShowName();
    else if(toolName == Cfhs_AutoRegionConfig_hjh::getToolName())
        showName = Cfhs_AutoRegionConfig_hjh::getShowName();
    else if(toolName == Cfhs_DefectConfig_hjh::getToolName())
        showName = Cfhs_DefectConfig_hjh::getShowName();

    return showName;
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

void Cfhs_StationSingleTool::setToolName(const QString &name)
{
    m_stationTool.m_toolName = name;
    QString showName = getShowName(name);
    QString tip = getToolTip(name);
    QString iconPath = getIconPath(name);
    if(m_stationTool.m_times > 0)
        showName = QString("%1_%2").arg(showName).arg(m_stationTool.m_times);

    setShowName(showName);
    this->setToolTip(tip);
    QIcon icon;
    icon.addFile(iconPath);
    this->setIcon(icon);
}
