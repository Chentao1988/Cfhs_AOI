#ifndef CFHS_YIELDWARNING_H
#define CFHS_YIELDWARNING_H

#include <QDialog>
#include <QPointer>
#include "cfhs_base.h"


class QLabel;
class QSpinBox;
class QPushButton;

//该类主要用于良率的实时预警
class Cfhs_YieldWarning : public QDialog
{
    Q_OBJECT

public:
    Cfhs_YieldWarning(QWidget *parent = nullptr);
    ~Cfhs_YieldWarning();
    void setYieldValue(const int &startVal, const QString &strWarning);
    void getYieldValue(int &startVal, QString &strWarning);

private slots:
    void commitButton_clicked();
    void cancelButton_clicked();

private:
    QPointer<QLabel> m_countLabel1;
    QPointer<QLabel> m_countLabel2;
    QPointer<QSpinBox> m_countSpinbox;  //开始计算值
    QPointer<QLabel> m_promptLabel1;
    QPointer<QLabel> m_promptLabel2;
    QPointer<QSpinBox> m_promptSpinbox;  //良率提示值
    QPointer<QPushButton> m_commitButton;  //确定
    QPointer<QPushButton> m_cancelButton;  //取消
};

#endif // CFHS_YIELDWARNING_H
