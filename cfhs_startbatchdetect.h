#ifndef CFHS_STARTBATCHDETECT_H
#define CFHS_STARTBATCHDETECT_H

#include <QDialog>
#include <QPointer>
#include "cfhs_base.h"


class QLineEdit;
class Cfhs_ComboBox;
class QPushButton;
class QLabel;



//该类主要用于插入批次，并进行检测工作

class Cfhs_StartBatchDetect : public QDialog
{
    Q_OBJECT

public:
    Cfhs_StartBatchDetect(QWidget* parent = nullptr);
    ~Cfhs_StartBatchDetect();
    void setBatchInfo(const QString& nowBatch, const QStringList& listBatch);
    void setBatchList(const QStringList& listBatch);
    QString getNowBatchName() const;

private slots:
    void insertRecordButton_clicked();
    void startButton_clicked();
    void cancelButton_clicked();


private:
    QPointer<QLabel> m_curRecordLabel;
    QPointer<QLineEdit> m_curRecordLineEdit;  //当前批次
    QPointer<QLabel> m_alreadyRecodeLabel;
    QPointer<Cfhs_ComboBox> m_alreadyRecodeCombo;  //已有批次
    QPointer<QPushButton> m_insertRecordButton;  //插入批次
    QPointer<QPushButton> m_startButton;  //开始
    QPointer<QPushButton> m_cancelButton;  //取消
};

#endif // CFHS_STARTBATCHDETECT_H
