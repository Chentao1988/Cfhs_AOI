#ifndef CFHS_DISKMONITOR_H
#define CFHS_DISKMONITOR_H

#include <QDialog>
#include <QPointer>
#include "cfhs_base.h"



class QSpinBox;
class QPushButton;
class QLabel;

//该类主要用于磁盘空间监控
class Cfhs_DiskMonitor : public QDialog
{
    Q_OBJECT

public:
    Cfhs_DiskMonitor(QWidget* parent = nullptr);
    ~Cfhs_DiskMonitor();
    void setDiskMonitor(const int &warnVal, const int &deleteVal);
    void getDiskMonitor(int &warnVal, int &deleteVal);
private slots:
    void commitButton_clicked();
    void cancelButton_clicked();

private:
    QPointer<QLabel> m_diskDWarningLabel1;
    QPointer<QLabel> m_diskDWarningLabel2;
    QPointer<QSpinBox> m_diskDWarning; //D盘预警
    QPointer<QLabel> m_diskDDeleteLabel1;
    QPointer<QLabel> m_diskDDeleteLabel2;
    QPointer<QSpinBox> m_diskDDelete;  //D盘删除
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
};

#endif // CFHS_DISKMONITOR_H
