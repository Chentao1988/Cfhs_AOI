#ifndef CFHS_SOFTMAINTAIN_H
#define CFHS_SOFTMAINTAIN_H

#include <QDialog>
#include <QPointer>
#include "cfhs_base.h"



class QLineEdit;
class QCheckBox;
class QPushButton;
class QLabel;
class QGroupBox;


//该类主要用于软件维护设置
class Cfhs_SoftMaintain : public QDialog
{
    Q_OBJECT

public:
    Cfhs_SoftMaintain(QWidget* parent = nullptr);
    ~Cfhs_SoftMaintain();

private slots:
    void commitButton_clicked();
    void cancelButton_clicked();

private:
    QPointer<QLabel> m_systemTestTimeLabel;
    QPointer<QLineEdit> m_systemTestTime;   //系统自检周期
    QPointer<QLabel> m_fileExpiredTimeLabel;
    QPointer<QLineEdit> m_fileExpiredTime;  //文件过期时限
    QPointer<QCheckBox> m_bMaintainPrompt;  //自动维护前是否提示
    QPointer<QCheckBox> m_bSmallImageDelete; //删除小图
    QPointer<QCheckBox> m_bStatisticsDelete;  //删除统计
    QPointer<QCheckBox> m_bLogDelete;  //删除日志
    QPointer<QCheckBox> m_bDatabaseDelete;  //删除数据库
    QPointer<QPushButton> m_commitButton;  //确定
    QPointer<QPushButton> m_cancelButton;  //取消
    QPointer<QGroupBox> m_deleteGroup;  //选择删除类别
};

#endif // CFHS_SOFTMAINTAIN_H
