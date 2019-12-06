#ifndef CFHS_ROICONFIG_H
#define CFHS_ROICONFIG_H

#include <QDialog>
#include <QPointer>

class QRadioButton;
class QPushButton;

class Cfhs_RoiConfig : public QDialog
{
    Q_OBJECT

public:
    Cfhs_RoiConfig(QWidget *parent = nullptr);
    ~Cfhs_RoiConfig();
    QString getParaConfig() const;
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolPosition(); //工具位置，2-1 前位：模块索引，后位：该模块中的工具索引
    bool setParaConfig(const QString& strConfig);

private slots:
    void on_commitButton_clicked();
    void on_cancelButton_clicked();

private:
    QString m_strConfig;
    QPointer<QRadioButton> m_autoButton;
    QPointer<QRadioButton> m_manualButton;
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
};

#endif // CFHS_ROICONFIG_H
