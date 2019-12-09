﻿#ifndef CFHS_ITODETECTCONFIG_H
#define CFHS_ITODETECTCONFIG_H

#include <QDialog>
#include <QPointer>
#include "../cfhs_base.h"

class QPushButton;
class QLabel;
class Cfhs_AlgorithmTable;

//该类用于设置算法“ITO检测”的参数
class Cfhs_ItoDetectConfig : public QDialog
{
public:
    Cfhs_ItoDetectConfig(QWidget *parent = nullptr);
    ~Cfhs_ItoDetectConfig();
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolPosition(); //工具位置，2-1 前位：模块索引，后位：该模块中的工具索引
    static QString getToolParaDefault();  //默认参数
    static QStringList getToolOutput(const LanguageEnum &language);  //获取算子输出参数列表
    QString getParaConfig() const;
    bool setParaConfig(const QString& strConfig);


private:
    //根据参数名获取参数位置索引
    int getIndexFromName(const QString& name);
    //根据参数位置索引获取参数名，从1开始
    QString getNameFromIndex(const int& index);
    //根据参数名获取参数在表格上的行数
    int getRowFromName(const QString &name);

private slots:
    void onCommitButton_clicked();
    void onCancelButton_clicked();

private:
    QPointer<QLabel> m_nameLabel;  //功能名
    QPointer<Cfhs_AlgorithmTable> m_algoTable; //算法参数表
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
    QString m_strConfig;
};

#endif // CFHS_ITODETECTCONFIG_H