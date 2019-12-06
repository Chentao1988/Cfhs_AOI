#ifndef CFHS_ORIGINALREGIONCONFIG_H
#define CFHS_ORIGINALREGIONCONFIG_H

#include <QObject>

class Cfhs_OriginalRegionConfig : public QObject
{
    Q_OBJECT
public:
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolPosition(); //工具位置，2-1 前位：模块索引，后位：该模块中的工具索引
};

#endif // CFHS_ORIGINALREGIONCONFIG_H
