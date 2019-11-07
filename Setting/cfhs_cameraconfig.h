﻿#ifndef CFHS_CAMERACONFIG_H
#define CFHS_CAMERACONFIG_H

#include <QDialog>
#include <QPointer>
#include <QMap>


class QTreeWidget;
class QLabel;
class QPushButton;
class Cfhs_ComboBox;
class QTreeWidgetItem;

class Cfhs_CameraConfig : public QDialog
{
    Q_OBJECT

public:
    Cfhs_CameraConfig(QWidget *parent = nullptr);
    ~Cfhs_CameraConfig();
    void setCameraConfig(const QString &strConfig);
    QString getParaConfig() const;
    void setParaConfig(const QString &strPara);
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolName(); //工具名 固定英文

private:
    QTreeWidgetItem *getItem(const QString &name,
                             const bool &hasIcon = false,
                             const int &column = 0);

private slots:
    void on_cameraTree_itemClicked(QTreeWidgetItem *item, int column);
    void on_commitButton_clicked();
    void on_cancelButton_clicked();

private:
    QString m_strConfig;
    QPointer<QTreeWidget> m_cameraTree;
    QPointer<QLabel> m_ccfNameLabel;
    QPointer<Cfhs_ComboBox> m_ccfCombo;
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
    QString m_strCameraType;
    QString m_strCcfFile;
    //key  相机品牌
    //value  Map：key 相机型号   value CCF文件
    QMap<QString, QMap<QString, QStringList>> m_mapCamera;
};

#endif // CFHS_CAMERACONFIG_H
