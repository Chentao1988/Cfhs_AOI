#ifndef CFHS_ADDCAMERAINFO_H
#define CFHS_ADDCAMERAINFO_H

#include <QDialog>
#include <QPointer>
#include <QMap>

class Cfhs_ComboBox;
class QLabel;
class QPushButton;
class lineedit_button;


//添加相机记录
class Cfhs_AddCameraInfo : public QDialog
{
    Q_OBJECT

public:
    Cfhs_AddCameraInfo(QWidget *parent = nullptr);
    ~Cfhs_AddCameraInfo();
    int DialogShow();

private:
    QStringList getKeyListFromMap(const QMap<QString, QStringList> &map);
    QStringList getKeyListFromMap(const QMap<QString, QMap<QString, QStringList>> &map);

private slots:
    void on_cameraBrandCombo_currentIndex_changed(const QString &text);
    void on_cameraBrandCombo_eidtText_changed(const QString &text);
    void on_commitButton_clicked();
    void on_cancelButton_clicked();

private:
    QPointer<QLabel> m_cameraBrandLabel;  //相机品牌
    QPointer<Cfhs_ComboBox> m_cameraBrandCombo;
    QPointer<QLabel> m_cameraTypeLabel; //相机型号
    QPointer<Cfhs_ComboBox> m_cameraTypeCombo;
    QPointer<lineedit_button> m_ccfLineEdit;  //CCF配置文件
    QPointer<QPushButton> m_commitButton;  //确定
    QPointer<QPushButton> m_cancelButton;  //取消
    //key  相机品牌
    //value  Map：key 相机型号   value CCF文件
    QMap<QString, QMap<QString, QStringList>> m_mapCamera;
};

#endif // CFHS_ADDCAMERAINFO_H
