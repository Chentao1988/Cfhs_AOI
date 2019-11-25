#include "cfhs_softwareversion.h"
#include "cfhs_base.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>


Cfhs_SoftwareVersion::Cfhs_SoftwareVersion(QWidget *parent)
    :QDialog(parent)
{
    this->init();
    this->setWindowStyle();
}

Cfhs_SoftwareVersion::~Cfhs_SoftwareVersion()
{

}

void Cfhs_SoftwareVersion::init()
{
    //公司logo
    m_logoLabel = new QLabel(this);
    //软件版本
    m_versionLabel = new QLabel(tr("软件版本："));
    m_versionLineEdit = new QLineEdit(this);
    m_versionLineEdit->setReadOnly(true);
    //m_versionLineEdit->setText("V2.2.0620");
    m_versionLineEdit->setText("V2.1.0620");
    QPointer<QHBoxLayout> versionLayout = new QHBoxLayout;
    versionLayout->addWidget(m_versionLabel);
    versionLayout->addWidget(m_versionLineEdit,1);
    //出厂日期
    m_releaseTimeLabel = new QLabel(this);
    QString strRelease = "Copyright 2019 Cfhs(shenzhen) Company Ltd. All rights reserved.";
    m_releaseTimeLabel->setText(strRelease);
    //加密狗到期日期
    m_lockLabel = new QLabel(tr("加密狗到期日期："), this);
    m_lockLineEdit = new QLineEdit(this);
    m_lockLineEdit->setReadOnly(true);
    m_lockLineEdit->setText("2020-02-29");
    m_addLockTimeButton = new QPushButton(this);
    m_addLockTimeButton->setText(tr("续期"));
    connect(m_addLockTimeButton, &QPushButton::clicked,
            this, &Cfhs_SoftwareVersion::on_addLockTimeButton_clicked);
    QPointer<QHBoxLayout> lockLayout = new QHBoxLayout;
    lockLayout->addWidget(m_lockLabel);
    lockLayout->addWidget(m_lockLineEdit, 1);
    lockLayout->addWidget(m_addLockTimeButton);
    //联系人
    m_contactManLabel = new QLabel(tr("联系人："), this);
    m_contactManLineEdit = new QLineEdit(this);
    m_contactManLineEdit->setReadOnly(true);
    m_contactManLineEdit->setText(tr("廖工"));
    QPointer<QHBoxLayout> manLayout = new QHBoxLayout;
    manLayout->addWidget(m_contactManLabel);
    manLayout->addWidget(m_contactManLineEdit, 1);
    //联系方式
    m_contactPhoneLabel = new QLabel(tr("联系方式："), this);
    m_contactPhoneLineEdit = new QLineEdit(this);
    m_contactPhoneLineEdit->setReadOnly(true);
    m_contactPhoneLineEdit->setText("15820901331");
    QPointer<QHBoxLayout> phoneLayout = new QHBoxLayout;
    phoneLayout->addWidget(m_contactPhoneLabel);
    phoneLayout->addWidget(m_contactPhoneLineEdit, 1);
    //公司网址
    m_companyLabel = new QLabel(tr("公司网址："), this);
    m_companyButton = new QPushButton(this);
    //m_companyButton->setText("www.baidu.com");
    connect(m_companyButton, &QPushButton::clicked,
            this, &Cfhs_SoftwareVersion::on_companyButton_clicked);
    QPointer<QHBoxLayout> companyLayout = new QHBoxLayout;
    companyLayout->addWidget(m_companyLabel);
    companyLayout->addWidget(m_companyButton,1);
    //commit
    m_commitButton = new QPushButton(tr("确定"), this);
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_SoftwareVersion::on_commitButton_clicked);
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    //总layot
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_logoLabel, 0, Qt::AlignLeft|Qt::AlignVCenter);
    mainLayout->addLayout(versionLayout);
    mainLayout->addLayout(lockLayout);
    mainLayout->addLayout(manLayout);
    mainLayout->addLayout(phoneLayout);
    mainLayout->addLayout(companyLayout);
    mainLayout->addWidget(m_releaseTimeLabel, 0, Qt::AlignCenter);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(10, 20, 10, 20);
    this->setLayout(mainLayout);
    this->setWindowTitle(tr("版本相关"));
    this->resize(600, 500);
}

void Cfhs_SoftwareVersion::setWindowStyle()
{
    //logo
#if logo_cfdr
    m_logoLabel->setStyleSheet("QLabel{border-image: url(:/logo_cfdr.png);}");
    m_logoLabel->setFixedSize(52, 40);
#elif logo_shh
    m_logoLabel->setStyleSheet("QLabel{border-image: url(:/logo_shh.png);}");
    m_logoLabel->setFixedSize(200, 67);
#endif
    //网址
    m_companyButton->setStyleSheet("QPushButton{background:transparent; border:none;"
                                   "color:#0077FF; font-size:24px;"
                                   "font-family:Mircosoft Yahei;}"
                                   "QPushButton:pressed{color:#2193AB;}");
}

void Cfhs_SoftwareVersion::on_commitButton_clicked()
{
    this->accept();
}

void Cfhs_SoftwareVersion::on_addLockTimeButton_clicked()
{

}

void Cfhs_SoftwareVersion::on_companyButton_clicked()
{
    QString text = m_companyButton->text();
    if(text.isEmpty())
        return;
    QDesktopServices::openUrl(QUrl(text));
}
