#include "cfhs_originalregionconfig.h"
#include "cfhs_base.h"


QString Cfhs_OriginalRegionConfig::getShowName()
{
    QString name = tr("原图分区");

    return name;
}

QString Cfhs_OriginalRegionConfig::getToolTip()
{
    QString tip = QString(tr("对原图进行检测，不分区"));
    return tip;
}

QString Cfhs_OriginalRegionConfig::getIconPath()
{
    QString path = ":/image_tool/auto_region_config.png";

    return path;
}

QString Cfhs_OriginalRegionConfig::getToolPosition()
{
    return "2-3";
}
