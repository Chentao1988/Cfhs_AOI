#include "cfhs_insertrecord.h"
#include "cfhs_base.h"
#include "cfhs_combobox.h"
#include "cfhs_global.h"
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QDateTime>
#include <QMessageBox>


Cfhs_InsertRecord::Cfhs_InsertRecord(QWidget* parent)
    : QDialog(parent)
{
    //班组
    m_groupLabel = new QLabel(this);
    m_groupLabel->setText(tr("班组："));
    m_groupCombo = new Cfhs_ComboBox(this);
    m_groupCombo->addItem(tr("白班"));
    m_groupCombo->addItem(tr("夜班"));
    m_groupCombo->setCurrentIndex(0);
    //工号
    m_staffNoLabel = new QLabel(this);
    m_staffNoLabel->setText(tr("工号："));
    m_staffNoSpinbox = new QSpinBox(this);
    m_staffNoSpinbox->setRange(1, 99);
    //次数
    m_timesLabel = new QLabel(this);
    m_timesLabel->setText(tr("次数："));
    m_timesSpinbox = new QSpinBox(this);
    m_timesSpinbox->setRange(1, 9999);
    //添加layout
    QPointer<QGridLayout> topLayout = new QGridLayout();
    topLayout->addWidget(m_groupLabel, 0, 0);
    topLayout->addWidget(m_groupCombo, 0, 1);
    topLayout->addWidget(m_staffNoLabel, 1, 0);
    topLayout->addWidget(m_staffNoSpinbox, 1, 1);
    topLayout->addWidget(m_timesLabel, 2, 0);
    topLayout->addWidget(m_timesSpinbox, 2, 1);
    topLayout->setSpacing(20);
    //确定button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_InsertRecord::commitButton_clicked);
    //取消button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_InsertRecord::cancelButton_clicked);
    //添加layout
    QPointer<QHBoxLayout> bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(m_cancelButton);
    bottomLayout->addWidget(m_commitButton);
    //给主界面添加layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->setContentsMargins(10, 15, 10, 20);
    mainLayout->setSpacing(20);
    this->setLayout(mainLayout);
    //设置主界面属性
    this->setWindowTitle(tr("新建批次"));
    this->resize(400, 300);
}

Cfhs_InsertRecord::~Cfhs_InsertRecord()
{

}

QString Cfhs_InsertRecord::getInsertBatchName() const
{
    return m_insertBatchName;
}

void Cfhs_InsertRecord::commitButton_clicked()
{
    //批次名 时间_班次_工号_次数
    //时间  yyMMddhhmm
    //班次  白班 A，夜班 B
    QString strTime  = QDateTime::currentDateTime().toString("yyMMddhhmm");
    QString strGroup = "";
    if(m_groupCombo->currentIndex() == 0)
        strGroup = "A";
    else
        strGroup = "B";
    char dest[4] = {0};
    sprintf(dest, "%02d", m_staffNoSpinbox->value());
    QString strNo = QString(dest);
    sprintf(dest, "%04d", m_timesSpinbox->value());
    QString strNum = QString(dest);
    QString batchName = QString("%1_%2_%3_%4")
            .arg(strTime)
            .arg(strGroup)
            .arg(strNo)
            .arg(strNum);
    //插入批次
    QString strInfo;
    if(!m_logicInterface->InsertNewBatchName(batchName, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    m_insertBatchName = batchName;
    this->accept();
}

void Cfhs_InsertRecord::cancelButton_clicked()
{
    this->reject();
}
