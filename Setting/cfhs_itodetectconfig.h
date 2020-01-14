#ifndef CFHS_ITODETECTCONFIG_H
#define CFHS_ITODETECTCONFIG_H

#include "cfhs_toolbase.h"

class QPushButton;
class QLabel;
class Cfhs_AlgorithmTable;

//该类用于设置算法“ITO检测”的参数
class Cfhs_ItoDetectConfig : public Cfhs_ToolBase
{
    Q_OBJECT

public:
    Cfhs_ItoDetectConfig(QWidget *parent = nullptr);
    ~Cfhs_ItoDetectConfig();
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolPosition(); //工具位置，2-1 前位：模块索引，后位：该模块中的工具索引
    static QString getToolParaDefault();  //默认参数
    static QStringList getToolOutput(const LanguageEnum &language);  //获取算子输出参数列表
    //获取算法参数
    virtual QString getParaConfig();
    //设置算法参数
    //输入 strConfig  json格式
    virtual bool setParaConfig(const QString& strConfig);
};

#endif // CFHS_ITODETECTCONFIG_H
