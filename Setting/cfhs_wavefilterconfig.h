#ifndef CFHS_WAVEFILTERCONFIG_H
#define CFHS_WAVEFILTERCONFIG_H

#include "cfhs_toolbase.h"


//算法膨胀滤波
class Cfhs_WaveFilterConfig : public Cfhs_ToolBase
{
    Q_OBJECT

public:
    Cfhs_WaveFilterConfig(QWidget *parent = nullptr);
    ~Cfhs_WaveFilterConfig();
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolPosition(); //工具位置，2-1 前位：模块索引，后位：该模块中的工具索引
    //获取算法参数
    virtual QString getParaConfig();
    //设置算法参数
    //输入 strConfig  json格式
    virtual bool setParaConfig(const QString& strConfig);
};

#endif // CFHS_WAVEFILTERCONFIG_H
