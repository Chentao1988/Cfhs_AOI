#include "cfhs_logindialog.h"
#include "cfhs_pwdmodifydialog.h"
#include "cfhs_global.h"
#include "cfhs_combobox.h"
#include <QPixmap>
#include <QIcon>
#include <QMessageBox>
#include <QColor>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>


Cfhs_LoginDialog::Cfhs_LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    this->init();
    this->setWindowStyle();
}

Cfhs_LoginDialog::~Cfhs_LoginDialog()
{

}

UserType Cfhs_LoginDialog::getUser()
{
    return m_user;
}

PermissionEnum Cfhs_LoginDialog::getUserPermission()
{
    return m_user.m_Permission;
}

void Cfhs_LoginDialog::setWindowStyle()
{
    //logo显示
    m_logoLabel->setStyleSheet("QLabel{border-image: url(:/logo.png);}");
    m_logoLabel->setFixedSize(52, 40);
    //标题1
    m_title1Label->setStyleSheet("QLabel{font-size:36px; font-family:Mircosoft Yahei;"
                                 "font-weight:bold; color:white;border-image:none;}");
    //标题2
    m_title2Label->setStyleSheet("QLabel{font-size:30px; font-family:Mircosoft Yahei;"
                                 "font-weight:bold; color:white;border-image:none;}");
    //Frame
    m_showFrame->setStyleSheet("QFrame{border-image: url(:/login_show.png)}");
    //账号
    m_userComboBox->setStyleSheet("QComboBox{border-image: url(:/line_edit_user.png);"
                                  "background:transparent;border:none;color:#0077FF;"
                                  "font-size:18px; font-family:Mircosoft Yahei;"
                                  "padding-left:50px; font-weight:blod;border-radius:0px;}"
                                  "QComboBox:hover{color: black;}"
                                  "QComboBox::drop-down{background:transparent; border:none;}"
                                  "QComboBox::down-arrow{image: url(:/login_combo_arrow.png)}"
                                  "QListView{background:white;border:none;}"
                                  "QListView::item{background:transparent; color:#0077FF;"
                                  "border-left:1px solid #c2c2c2; font-size:18px;"
                                  "border-bottom: 1px solid #c2c2c2;height:30px;"
                                  "font-family:Mircosoft Yahei;}"
                                  "QListView::item:selected{background:#0077FF; color:white;}");
    m_userComboBox->setFixedSize(300, 32);
    m_userLabel->setFixedSize(100, 32);
    //密码
    m_pwdLineEdit->setStyleSheet("QLineEdit{border-image: url(:/line_edit_password.png);"
                                 "background:transparent;border:none;color:#0077FF;"
                                 "font-size:18px; font-family:Mircosoft Yahei;"
                                 "padding-left:50px; font-weight:blod;}"
                                 "QLineEdit:hover{color: black;}"
                                 "QLineEdit:disabled{background:lightGray;}");
    m_pwdLineEdit->setFixedSize(300, 32);
    //修改密码
    m_modifyPwdButton->setStyleSheet("QPushButton{background:transparent; border:none;"
                                     "color:#0077FF; font-size:18px; font-family:Mircosoft Yahei;"
                                     "padding: 2px 2px;}"
                                     "QPushButton:pressed{color:black;}");
    m_modifyPwdButton->setFixedSize(100, 32);
    //登录
    m_loginButton->setStyleSheet("QPushButton{background:#0077FF; border:none;"
                                 "color:white; font-size:26px; font-family:Mircosoft Yahei;"
                                 "padding: 2px 2px; border-radius: 6px; min-height: 40px;}"
                                 "QPushButton:pressed{color:lightGray;}");
    //主界面
    this->setStyleSheet("Cfhs_LoginDialog{background: white; border:none}");
    this->setFixedSize(600, 420);
}

void Cfhs_LoginDialog::init()
{
    this->setWindowFlags(this->windowFlags() | Qt::WindowMinimizeButtonHint);
    //公司名
    QString name = QString(tr("创富华视"));
    this->setWindowTitle(name);
    //公司logo
    m_logoLabel = new QLabel(this);
    //标题1
    m_title1Label = new QLabel(this);
    m_title1Label->setText(tr("屏幕缺陷检测"));
    //标题2
    m_title2Label = new QLabel(this);
    m_title2Label->setText(tr("科技提升产品质量，创造客户价值"));
    //Frame
    m_showFrame = new QFrame(this);
    QPointer<QVBoxLayout> showLayout = new QVBoxLayout();
    showLayout->addWidget(m_logoLabel, 0, Qt::AlignHCenter|Qt::AlignLeft);
    showLayout->addWidget(m_title1Label, 0, Qt::AlignCenter);
    showLayout->addWidget(m_title2Label, 0, Qt::AlignCenter);
    showLayout->setContentsMargins(0,0,0,30);
    showLayout->setSpacing(20);
    m_showFrame->setLayout(showLayout);
    //账号
    m_userLabel = new QLabel(this);
    QStringList userList;
    userList.append(tr("管理员"));
    userList.append(tr("操作员"));
    m_userComboBox = new Cfhs_ComboBox(this);
    m_userComboBox->addItems(userList);
    m_userComboBox->setCurrentIndex(0);
    QPointer<QHBoxLayout> userLayout = new QHBoxLayout();
    userLayout->addStretch(2);
    userLayout->addWidget(m_userComboBox);
    userLayout->addWidget(m_userLabel);
    userLayout->addStretch(1);
    userLayout->setSpacing(10);
    //密码
    m_pwdLineEdit = new QLineEdit(this);
    m_pwdLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    m_pwdLineEdit->setFocus();
    //修改密码
    m_modifyPwdButton = new QPushButton(tr("修改密码"), this);
    QPointer<QHBoxLayout> pwdLayout = new QHBoxLayout();
    pwdLayout->addStretch(2);
    pwdLayout->addWidget(m_pwdLineEdit);
    pwdLayout->addWidget(m_modifyPwdButton);
    pwdLayout->addStretch(1);
    pwdLayout->setSpacing(10);
    //登录
    m_loginButton = new QPushButton(tr("登录"), this);
    m_loginButton->setDefault(true);
    QPointer<QVBoxLayout> bottomLayout = new QVBoxLayout;
    bottomLayout->addLayout(userLayout);
    bottomLayout->addLayout(pwdLayout);
    bottomLayout->addWidget(m_loginButton);
    bottomLayout->setSpacing(20);
    bottomLayout->setContentsMargins(20, 0, 20, 0);
    //添加主layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_showFrame, 1);
    mainLayout->addLayout(bottomLayout);
    mainLayout->setSpacing(30);  //添加30px的间隙
    mainLayout->setContentsMargins(0,0,0,20);
    this->setLayout(mainLayout);
    on_userComboBox_currentIndexChanged(0);
    //连接信号槽
    //connect(m_userComboBox, currentIndexChanged,
            //this, &Cfhs_LoginDialog::on_userComboBox_currentIndexChanged);
    connect(m_userComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_userComboBox_currentIndexChanged(int)));
    connect(m_modifyPwdButton, &QPushButton::clicked,
            this, &Cfhs_LoginDialog::on_modifyPwdButton_clicked);
    connect(m_loginButton, &QPushButton::clicked,
            this, &Cfhs_LoginDialog::on_loginButton_clicked);
}

void Cfhs_LoginDialog::on_modifyPwdButton_clicked()
{
    Cfhs_PwdModifyDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted)
    {
        m_user.m_password = dialog.getPassword();
        QMessageBox::information(this, " ", tr("密码修改成功"));
    }
}

void Cfhs_LoginDialog::on_loginButton_clicked()
{
    if(m_user.m_Permission == AdminPermission)
    {
        QString pd = m_pwdLineEdit->text();
        if(pd.isEmpty())
        {
            QMessageBox::information(this, " ", tr("请输入密码"));
            m_pwdLineEdit->setFocus();
            return;
        }
        QString strInfo;
        if(!m_logicInterface->Login(pd, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
    }
    this->accept();
}

void Cfhs_LoginDialog::on_userComboBox_currentIndexChanged(int index)
{
    if(index < 0)
        return;

    //密码输入框是否可用
    bool isUsed = false;
    switch(index)
    {
    case 0:
        isUsed = true;
        m_user.m_Permission = AdminPermission;
        break;
    case 1:
        isUsed = false;
        m_user.m_Permission = OperatorPermission;
        break;
    }
    m_user.m_name = m_userComboBox->currentText();
    m_pwdLineEdit->setEnabled(isUsed);
    if(!isUsed)
        m_pwdLineEdit->clear();
}
