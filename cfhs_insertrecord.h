#ifndef CFHS_INSERTRECORD_H
#define CFHS_INSERTRECORD_H

#include "cfhs_base.h"
#include <QDialog>
#include <QPointer>

class QPushButton;
class QLabel;
class QSpinBox;
class Cfhs_ComboBox;


class Cfhs_InsertRecord : public QDialog
{
    Q_OBJECT

public:
    Cfhs_InsertRecord(QWidget* parent = nullptr);
    ~Cfhs_InsertRecord();
    QString getInsertBatchName() const;

private slots:
    void commitButton_clicked();
    void cancelButton_clicked();

private:
    QPointer<QLabel> m_groupLabel;  //班组
    QPointer<Cfhs_ComboBox> m_groupCombo;
    QPointer<QLabel> m_staffNoLabel;  //工号
    QPointer<QSpinBox> m_staffNoSpinbox;
    QPointer<QLabel> m_timesLabel;  //次数
    QPointer<QSpinBox> m_timesSpinbox;
    QPointer<QPushButton> m_commitButton;  //确定
    QPointer<QPushButton> m_cancelButton;  //取消
    QString m_insertBatchName;
};

#endif // CFHS_INSERTRECORD_H
