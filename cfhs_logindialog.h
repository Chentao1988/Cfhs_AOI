﻿#ifndef CFHS_LOGINDIALOG_H
#define CFHS_LOGINDIALOG_H

#include <QDialog>
#include <QPointer>
#include "cfhs_base.h"

class QLabel;
class QLineEdit;
class QPushButton;
class Cfhs_ComboBox;
class QFrame;
class QEvent;
class QTranslator;


class Cfhs_LoginDialog : public QDialog
{
    Q_OBJECT

public:
    //该类用于登录界面设计
    explicit Cfhs_LoginDialog(QWidget *parent = nullptr);
    ~Cfhs_LoginDialog();
    UserType getUser();
    PermissionEnum getUserPermission();
    int DialogShow();

protected:
    void changeEvent(QEvent *event);

private slots:

    void on_modifyPwdButton_clicked();

    void on_loginButton_clicked();

    void on_userComboBox_currentIndexChanged(int index);

    void on_langCombo_currentIndex_changed(int index);

private:
    void init();
    void setWindowStyle();
    bool updateUi(const int &index);


private:
    UserType m_user;
    QPointer<QLabel> m_logoLabel;  //公司Logo
    QPointer<QLabel> m_title1Label; //标题1
    QPointer<QLabel> m_title2Label; //标题2
    QPointer<Cfhs_ComboBox> m_userComboBox;
    QPointer<QLineEdit> m_pwdLineEdit;
    QPointer<QPushButton> m_modifyPwdButton;
    QPointer<QPushButton> m_loginButton;
    QPointer<QFrame> m_showFrame;
    QPointer<Cfhs_ComboBox> m_langCombo;  //语言切换
    QPointer<QTranslator> m_translator;  //翻译器
};

#endif // CFHS_LOGINDIALOG_H
