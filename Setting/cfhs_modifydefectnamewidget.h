#ifndef CFHS_MODIFYDEFECTNAMEWIDGET_H
#define CFHS_MODIFYDEFECTNAMEWIDGET_H

#include <QDialog>
#include <QPointer>

class QLabel;
class QLineEdit;
class QPushButton;

//该类主要用于修改缺陷名称
class Cfhs_ModifyDefectNameWidget : public QDialog
{
public:
    Cfhs_ModifyDefectNameWidget(const QString& curName, QWidget *parent = nullptr);
    ~Cfhs_ModifyDefectNameWidget();
    QString getDefectName();

private slots:
    void onCommitButton_clicked();
    void onCancelButton_clicked();

private:
    QString m_defectName;
    //当前缺陷名
    QPointer<QLabel> m_curNameLabel;  //标题
    QPointer<QLabel> m_curNameInfoLabel;  //内容
    //修改的缺陷名
    QPointer<QLabel> m_modifyNameLabel;  //标题
    QPointer<QLineEdit> m_modifyNameLineEdit;  //内容
    //确认 取消 button
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
};

#endif // CFHS_MODIFYDEFECTNAMEWIDGET_H
