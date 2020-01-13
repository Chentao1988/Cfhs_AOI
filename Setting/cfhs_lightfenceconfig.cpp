#include "cfhs_lightfenceconfig.h"
#include "../cfhs_global.h"


Cfhs_LightFenceConfig::Cfhs_LightFenceConfig(QWidget *parent)
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
