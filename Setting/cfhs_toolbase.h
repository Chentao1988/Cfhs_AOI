#ifndef CFHS_TOOLBASE_H
#define CFHS_TOOLBASE_H

#include "cfhs_global.h"
#include "cfhs_algorithmtable.h"
#include <QDialog>
#include <QPointer>
#include <QVector>

class QPushButton;
class QLabel;


//设置算法的公共类
class Cfhs_ToolBase : public QDialog
{
    Q_OBJECT

public:
    typedef struct _ParaInfo{
        int m_index;
        QString m_toolName;
        QString m_showName;
        QString m_toolTip;
    }ParaInfo;

    Cfhs_ToolBase(QWidget *parent = nullptr);
    ~Cfhs_ToolBase();
    //Label样式设置
    virtual QString getToolLabelStyle();
    //获取算法参数  定义标准接口
    virtual QString getParaConfig() = 0;
    //设置算法参数  定义标准接口
    //输入 strConfig  json格式
    virtual bool setParaConfig(const QString& strConfig) = 0;
    void setFunctionName(const QString &name);
    void setParaVector(const QVector<Cfhs_ToolBase::ParaInfo> &vector);
    //获取算子的参数个数
    int getParaNum() const;
    //插入一行参数
    //输入
    //toolName 工具名，固定英文名
    //showName 工具显示名
    //toolTip  工具介绍
    void insertOnePara(const QString &toolName, const QString &showName, const QString &toolTip);

protected:
    //根据参数名获取参数位置索引
    int getIndexFromName(const QString& name);
    //根据参数位置索引获取参数名，从1开始
    QString getNameFromIndex(const int& index);
    //根据参数名获取参数在表格上的行数
    int getRowFromName(const QString &name);

private slots:
    void onCommitButton_clicked();
    void onCancelButton_clicked();

protected:
    QPointer<Cfhs_AlgorithmTable> m_algoTable; //算法参数表
    QString m_strConfig;
    QVector<ParaInfo> m_vectorPara;

private:
    QPointer<QLabel> m_nameLabel;  //功能名
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
};

#endif // CFHS_TOOLBASE_H
