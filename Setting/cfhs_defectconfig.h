#ifndef CFHS_DEFECTCONFIG_H
#define CFHS_DEFECTCONFIG_H

#include <QDialog>
#include <QPointer>

class QLabel;
class QPushButton;
class Cfhs_AlgorithmTable;


//算法瑕疵检测
class Cfhs_DefectConfig : public QDialog
{
    Q_OBJECT

public:
    Cfhs_DefectConfig(QWidget *parent = nullptr);
    ~Cfhs_DefectConfig();
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolName(); //工具名 固定英文
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

#endif // CFHS_DEFECTCONFIG_H
