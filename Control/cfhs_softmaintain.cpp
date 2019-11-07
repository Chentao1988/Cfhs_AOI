#include "cfhs_softmaintain.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>


Cfhs_SoftMaintain::Cfhs_SoftMaintain(QWidget* parent)
    : QDialog(parent)
{
    //系统自检周期
    m_systemTestTimeLabel = new QLabel(this);
    m_systemTestTimeLabel->setText(tr("系统自检周期"));
    m_systemTestTime = new QLineEdit(this);
    QHBoxLayout* systemTestLayout = new QHBoxLayout();
    systemTestLayout->addWidget(m_systemTestTimeLabel);
    systemTestLayout->addWidget(m_systemTestTime);
    //文件过期时限
    m_fileExpiredTimeLabel = new QLabel(this);
    m_fileExpiredTimeLabel->setText(tr("文件过期时限"));
    m_fileExpiredTime = new QLineEdit(this);
    QHBoxLayout* fileExpiredLayout = new QHBoxLayout();
    fileExpiredLayout->addWidget(m_fileExpiredTimeLabel);
    fileExpiredLayout->addWidget(m_fileExpiredTime);
    //自动维护前是否提示
    m_bMaintainPrompt = new QCheckBox(this);
    m_bMaintainPrompt->setText(tr("自动维护前是否提示"));
    //以上项添加layout
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addLayout(systemTestLayout);
    leftLayout->addLayout(fileExpiredLayout);
    leftLayout->addStretch();
    leftLayout->addWidget(m_bMaintainPrompt);
    leftLayout->addSpacing(20);
    //选择删除类别
    m_deleteGroup = new QGroupBox(this);
    m_deleteGroup->setTitle(tr("选择删除类别"));
    QVBoxLayout* deleteLayout = new QVBoxLayout();
    //删除小图
    m_bSmallImageDelete = new QCheckBox(this);
    m_bSmallImageDelete->setText(tr("删除小图"));
    //删除统计
    m_bStatisticsDelete = new QCheckBox(this);
    m_bStatisticsDelete->setText(tr("删除统计"));
    //删除日志
    m_bLogDelete = new QCheckBox(this);
    m_bLogDelete->setText(tr("删除日志"));
    //删除数据库数据
    m_bDatabaseDelete = new QCheckBox(this);
    m_bDatabaseDelete->setText(tr("删除数据库数据"));
    deleteLayout->addWidget(m_bSmallImageDelete);
    deleteLayout->addWidget(m_bStatisticsDelete);
    deleteLayout->addWidget(m_bLogDelete);
    deleteLayout->addWidget(m_bDatabaseDelete);
    m_deleteGroup->setLayout(deleteLayout);
    //上半部添加layout
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addLayout(leftLayout, 2);
    topLayout->addWidget(m_deleteGroup, 1);
    //确定
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_SoftMaintain::commitButton_clicked);
    //取消
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_SoftMaintain::cancelButton_clicked);
    //添加button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    //添加主界面layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addLayout(buttonLayout, 0);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(10, 30, 10, 30);
    this->setLayout(mainLayout);
    //主界面设置
    this->setWindowTitle(tr("系统自动维护设置"));
    this->resize(400, 300);
}

Cfhs_SoftMaintain::~Cfhs_SoftMaintain()
{

}

void Cfhs_SoftMaintain::commitButton_clicked()
{
    //do something
    this->accept();
}

void Cfhs_SoftMaintain::cancelButton_clicked()
{
    this->reject();
}
