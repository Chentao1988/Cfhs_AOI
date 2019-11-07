#ifndef CFHS_PWDMODIFYDIALOG_H
#define CFHS_PWDMODIFYDIALOG_H

#include <QDialog>
#include <QPointer>
#include "cfhs_base.h"


class QLineEdit;
class QPushButton;

class Cfhs_PwdModifyDialog : public QDialog
{
    Q_OBJECT

public:
    //该类为登录界面上的子界面，主要用于修改账号密码
    Cfhs_PwdModifyDialog(QWidget *parent = nullptr);
    ~Cfhs_PwdModifyDialog();
    QString getPassword();

private slots:
    void on_cancelButton_clicked();

    void on_commitButton_clicked();

private:
    QString m_passwordModify;
    QPointer<QLineEdit> m_curPassword;  //当前密码
    QPointer<QLineEdit> m_newPassword;  //新密码
    QPointer<QLineEdit> m_confirmPassword;  //确认密码
    QPointer<QPushButton> m_cancelButton;
    QPointer<QPushButton> m_commitButton;
};

#endif // CFHS_PWDMODIFYDIALOG_H
