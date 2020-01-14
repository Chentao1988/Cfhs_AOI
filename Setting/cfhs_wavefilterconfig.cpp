#include "cfhs_wavefilterconfig.h"
#include <QJsonDocument>
#include <QJsonObject>


Cfhs_WaveFilterConfig::Cfhs_WaveFilterConfig(QWidget *parent)
    : Cfhs_ToolBase(parent)
{
    //参数表
    insertOnePara("exception_dilate_element_width",
                  tr("滤波宽"), tr("膨胀操作核宽"));
    insertOnePara("exception_dilate_element_height",
                  tr("滤波高"), tr("膨胀操作核高"));
    insertOnePara("loop_stride_to_decide_stantard_grey_value",
                  ("灰阶取值段数"), tr("可接受灰度值遍历步长"));
    insertOnePara("grey_difference_negative",
                  tr("滤波低阈值"), tr("瑕疵检测可接受灰度下限"));
    insertOnePara("grey_difference_positive",
                  tr("滤波高阈值"), tr("瑕疵检测可接受灰度上限"));
    //参数名
    setFunctionName(tr("滤波参数"));
    //工具名
    setWindowTitle(getShowName());
    //窗体大小
    this->resize(600, 400);
}

Cfhs_WaveFilterConfig::~Cfhs_WaveFilterConfig()
{

}

QString Cfhs_WaveFilterConfig::getShowName()
{
    QString name = tr("膨胀滤波");

    return name;
}

QString Cfhs_WaveFilterConfig::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_WaveFilterConfig::getIconPath()
{
    QString path = ":/image_tool/wave_filter_config.png";

    return path;
}

QString Cfhs_WaveFilterConfig::getToolPosition()
{
    return "3-n";
}

QString Cfhs_WaveFilterConfig::getParaConfig()
{
    return "";
}

bool Cfhs_WaveFilterConfig::setParaConfig(const QString &strConfig)
{
    return false;
}
