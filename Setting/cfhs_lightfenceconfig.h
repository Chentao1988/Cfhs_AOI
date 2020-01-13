#ifndef CFHS_LIGHTFENCECONFIG_H
#define CFHS_LIGHTFENCECONFIG_H

#include <QDialog>

class Cfhs_LightFenceConfig : public QDialog
{
    Q_OBJECT

public:
    Cfhs_LightFenceConfig(QWidget *parent = nullptr);
    ~Cfhs_LightFenceConfig();
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolPosition(); //工具位置，2-1 前位：模块索引，后位：该模块中的工具索引
    static QString getToolParaDefault();  //默认参数
};

#endif // CFHS_LIGHTFENCECONFIG_H
