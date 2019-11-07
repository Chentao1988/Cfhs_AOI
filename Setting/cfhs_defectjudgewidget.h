#ifndef CFHS_DEFECTJUDGEWIDGET_H
#define CFHS_DEFECTJUDGEWIDGET_H

#include <QDialog>
#include <QPointer>
#include "Setting/cfhs_defectjudgetable.h"

class QPushButton;

//该类主要用于编辑缺陷判定方式
class Cfhs_DefectJudgeWidget : public QDialog
{
    Q_OBJECT

public:
    Cfhs_DefectJudgeWidget(const QString& winTitle,
                           const QStringList& nameList,
                           QWidget *parent = nullptr);
    ~Cfhs_DefectJudgeWidget();

private slots:
    void onAddButton_clicked();
    void onInsertButton_clicked();
    void onDelButton_clicked();
    void onSaveButton_clicked();

signals:
    void sendDefectInfo(const QString& info);

private:
    QPointer<QPushButton> m_addButton;
    QPointer<QPushButton> m_insertButton;
    QPointer<QPushButton> m_delButton;
    QPointer<QPushButton> m_saveButton;
    QPointer<Cfhs_DefectJudgeTable> m_defectTable;
};

#endif // CFHS_DEFECTJUDGEWIDGET_H
