#include "cfhs_calibrationconfig.h"
#include "../cfhs_global.h"



Cfhs_CalibrationConfig::Cfhs_CalibrationConfig(QWidget *parent)
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
