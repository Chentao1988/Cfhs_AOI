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
#include <QEvent>
#include <QTranslator>


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

void Cfhs_LoginDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        QString windownTitle = "";
        QString title1 = "";
        QString title2 = "";
        QString strPd = "";
        QString strLogin = "";
        QStringList listUser;
        if(m_currentLang == English)
        {
#if logo_cfdr
            windownTitle = "CFHS";
#elif logo_shh
            windownTitle = "SHH";
#endif
            title1 = "Screen defect detection";
            title2 = "Technology enhances product quality\n"
                     "      Creates customer value";
            strPd = "Password modify";
            strLogin = "Login";
            listUser.append("Administrator");
            listUser.append("Operator");
        }
        else if(m_currentLang == TraditionalChinese)
        {
#if logo_cfdr
            windownTitle = "創富華視";
#elif logo_shh
            windownTitle = "深鴻海";
#endif
            title1 = "屏幕缺陷檢測";
            title2 = "科技提升產品質量，創造客戶價值";
            strPd = "修改密碼";
            strLogin = "登錄";
            listUser.append("管理員");
            listUser.append("操作員");
        }
        else if(m_currentLang == SimplifiedChinese)
        {
#if logo_cfdr
            windownTitle = "创富华视";
#elif logo_shh
            windownTitle = "深鸿海";
#endif
            title1 = "屏幕缺陷检测";
            title2 = "科技提升产品质量，创造客户价值";
            strPd = "修改密码";
            strLogin = "登录";
            listUser.append("管理员");
            listUser.append("操作员");
        }
        this->setWindowTitle(windownTitle);
        m_title1Label->setText(title1);
        m_title2Label->setText(title2);
        m_modifyPwdButton->setText(strPd);
        m_loginButton->setText(strLogin);
        m_userComboBox->clear();
        m_userComboBox->addItems(listUser);
    }
}

void Cfhs_LoginDialog::setWindowStyle()
{
    //logo显示
#if logo_cfdr
    m_logoLabel->setStyleSheet("QLabel{border-image: url(:/logo_cfdr.png);}");
    m_logoLabel->setFixedSize(52, 40);
#elif logo_shh
    m_logoLabel->setStyleSheet("QLabel{border-image: url(:/logo_shh.png);}");
    m_logoLabel->setFixedSize(200, 67);
#endif
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
                                  "QComboBox::drop-down{background:transparent; border:none;}"
                                  "QComboBox::down-arrow{image: url(:/login_combo_arrow.png)}"
                                  "QListView{background:white;border:none;}"
                                  "QListView::item{background:transparent; color:#0077FF;"
                                  "border:1px solid #c2c2c2; font-size:18px;"
                                  "border-top: none;height:30px;"
                                  "font-family:Mircosoft Yahei;}"
                                  "QListView::item:selected{background:#0077FF; color:white;}");
    m_userComboBox->setFixedSize(300, 32);
    //语言切换
    m_langCombo->setStyleSheet("QComboBox{background:transparent;border:1px solid #c2c2c2;"
                                  "border-radius:6px; color:#0077FF; height:30px;"
                                  "font-size:18px; font-family:Mircosoft Yahei;"
                                  "padding-left:6px; font-weight:blod;border-radius:0px;}"
                                  "QComboBox::drop-down{background:transparent; border:none;}"
                                  "QComboBox::down-arrow{image: url(:/login_combo_arrow.png)}"
                                  "QListView{background:white;border:none;}"
                                  "QListView::item{background:transparent; color:#0077FF;"
                                  "border:1px solid #c2c2c2; font-size:18px;"
                                  "border-top: none;height:30px;"
                                  "font-family:Mircosoft Yahei;}"
                                  "QListView::item:selected{background:#0077FF; color:white;}");
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
                                     "padding: 0px 0px;}"
                                     "QPushButton:pressed{color:black;}");
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
#if logo_cfdr
    QString name = QString(tr("创富华视"));
#elif logo_shh
    QString name = QString(tr("深鸿海"));
#endif
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
    QStringList userList;
    userList.append(tr("管理员"));
    userList.append(tr("操作员"));
    m_userComboBox = new Cfhs_ComboBox(this);
    m_userComboBox->addItems(userList);
    m_userComboBox->setCurrentIndex(0);
    //语言切换
    m_langCombo = new Cfhs_ComboBox(this);
    QStringList langList;
    langList.append("English");
    langList.append("简体中文");
    langList.append("繁體中文");
    m_langCombo->addItems(langList);
    m_langCombo->setCurrentIndex(1);
    m_translator = new QTranslator(this);
    connect(m_langCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_langCombo_currentIndex_changed(int)));
    QPointer<QHBoxLayout> userLayout = new QHBoxLayout();
    userLayout->addWidget(m_userComboBox);
    userLayout->addWidget(m_langCombo);
    userLayout->setSpacing(20);
    //密码
    m_pwdLineEdit = new QLineEdit(this);
    m_pwdLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    m_pwdLineEdit->setFocus();
    //修改密码
    m_modifyPwdButton = new QPushButton(tr("修改密码"), this);
    QPointer<QHBoxLayout> pwdLayout = new QHBoxLayout();
    pwdLayout->addWidget(m_pwdLineEdit);
    pwdLayout->addWidget(m_modifyPwdButton);
    pwdLayout->setSpacing(20);
    QPointer<QVBoxLayout> contentLayout = new QVBoxLayout;
    contentLayout->addLayout(userLayout);
    contentLayout->addLayout(pwdLayout);
    QPointer<QHBoxLayout> midLayout = new QHBoxLayout;
    midLayout->addStretch();
    midLayout->addLayout(contentLayout);
    midLayout->addStretch();
    //登录
    m_loginButton = new QPushButton(tr("登录"), this);
    m_loginButton->setDefault(true);
    QPointer<QVBoxLayout> bottomLayout = new QVBoxLayout;
    bottomLayout->addLayout(midLayout);
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

void Cfhs_LoginDialog::on_langCombo_currentIndex_changed(int index)
{
    if(index < 0)
        return;
    switch (index)
    {
    case 0:
        m_currentLang = English;
        break;
    case 1:
        m_currentLang = SimplifiedChinese;
        break;
    case 2:
        m_currentLang = TraditionalChinese;
        break;
    default:
        m_currentLang = English;
        break;
    }
    //加载翻译器
    if(m_currentLang == SimplifiedChinese)
        qApp->removeTranslator(m_translator);
    else
    {
        QString path = "";
        if(m_currentLang == English)
        {
            path = QCoreApplication::applicationDirPath()+"/Resource/lang_En.qm";
            if(!m_translator->load(path))
            {
                QMessageBox::warning(this, " ", "Loading translator failed");
                return;
            }
        }
        else if(m_currentLang == TraditionalChinese)
        {
            path = QCoreApplication::applicationDirPath()+"/Resource/lang_Cht.qm";
            if(!m_translator->load(path))
            {
                QMessageBox::warning(this, " ", "加載翻譯文件失敗");
                return;
            }
        }
        //切换语言
        qApp->installTranslator(m_translator);
    }
    //更改数据库
    stConfig stConf;
    QString strInfo;
    if(!m_logicInterface->GetConfigInfo(stConf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    switch(m_currentLang)
    {
    case English:
        stConf.iSysLang = 1;
        break;
    case SimplifiedChinese:
        stConf.iSysLang = 0;
        break;
    case TraditionalChinese:
        stConf.iSysLang = 2;
        break;
    }
    if(!m_logicInterface->SetConfigInfo(stConf, strInfo))
        QMessageBox::warning(this, " ", strInfo);
}
