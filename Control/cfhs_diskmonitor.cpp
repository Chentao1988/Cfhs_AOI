#include "cfhs_diskmonitor.h"
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

const int warnMin = 5;
const int warnMax = 500;  //设置预警值
const int deleteMin = 1;  //设置自动删除值
const int deleteMax = 200;

Cfhs_DiskMonitor::Cfhs_DiskMonitor(QWidget* parent)
    : QDialog(parent)
{
    //D盘预警
    m_diskDWarningLabel1 = new QLabel(this);
    m_diskDWarningLabel1->setText(tr("设置D盘低于"));
    m_diskDWarningLabel2 = new QLabel(this);
    m_diskDWarningLabel2->setText(tr("GB提示报警"));
    m_diskDWarning = new QSpinBox(this);
    m_diskDWarning->setRange(warnMin, warnMax);
    //添加layout
    QPointer<QHBoxLayout> topLayout = new QHBoxLayout();
    topLayout->addWidget(m_diskDWarningLabel1);
    topLayout->addWidget(m_diskDWarning);
    topLayout->addWidget(m_diskDWarningLabel2);
    topLayout->setSpacing(30);
    //D盘删除
    m_diskDDeleteLabel1 = new QLabel(this);
    m_diskDDeleteLabel1->setText(tr("设置D盘低于"));
    m_diskDDeleteLabel2 = new QLabel(this);
    m_diskDDeleteLabel2->setText(tr("GB自动删除"));
    m_diskDDelete = new QSpinBox(this);
    m_diskDDelete->setRange(deleteMin, deleteMax);
    //添加layout
    QPointer<QHBoxLayout> bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(m_diskDDeleteLabel1);
    bottomLayout->addWidget(m_diskDDelete);
    bottomLayout->addWidget(m_diskDDeleteLabel2);
    bottomLayout->setSpacing(30);
    //确认button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确认"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_DiskMonitor::commitButton_clicked);
    //取消button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_DiskMonitor::cancelButton_clicked);
    //添加button layout
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addStretch();
    //添加主界面layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(40);
    mainLayout->setContentsMargins(10, 30, 10, 30);
    this->setLayout(mainLayout);
    //设置主界面
    this->setWindowTitle(tr("硬盘空间监控"));
    this->resize(500, 300);
}

Cfhs_DiskMonitor::~Cfhs_DiskMonitor()
{

}

void Cfhs_DiskMonitor::setDiskMonitor(const int &warnVal, const int &deleteVal)
{
    m_diskDWarning->setValue(warnVal);
    m_diskDDelete->setValue(deleteVal);
}

void Cfhs_DiskMonitor::getDiskMonitor(int &warnVal, int &deleteVal)
{
    warnVal = m_diskDWarning->value();
    deleteVal = m_diskDDelete->value();
}


void Cfhs_DiskMonitor::commitButton_clicked()
{
    this->accept();
}

void Cfhs_DiskMonitor::cancelButton_clicked()
{
    this->reject();
}
