#include "cfhs_startbatchdetect.h"
#include "cfhs_insertrecord.h"
#include "cfhs_combobox.h"
#include "cfhs_global.h"
#include "cfhs_base.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>



Cfhs_StartBatchDetect::Cfhs_StartBatchDetect(QWidget* parent)
    : QDialog(parent)
{
    //添加当前批次
    m_curRecordLabel = new QLabel(this);
    m_curRecordLabel->setText(tr("当前批次："));
    m_curRecordLineEdit = new QLineEdit(this);
    m_curRecordLineEdit->setReadOnly(true);
    QPointer<QHBoxLayout> curRecordLayout = new QHBoxLayout();
    curRecordLayout->addWidget(m_curRecordLabel, 0);
    curRecordLayout->addWidget(m_curRecordLineEdit, 1);    
    //已有批次
    m_alreadyRecodeLabel = new QLabel(this);
    m_alreadyRecodeLabel->setText(tr("已有批次："));
    m_alreadyRecodeCombo = new Cfhs_ComboBox(this);
    //给以上2个内容添加layout
    QPointer<QHBoxLayout> alreadRecordLayout = new QHBoxLayout();
    alreadRecordLayout->addWidget(m_alreadyRecodeLabel);
    alreadRecordLayout->addWidget(m_alreadyRecodeCombo, 1);
    //插入
    m_insertRecordButton = new QPushButton(this);
    m_insertRecordButton->setText(tr("插入"));
    connect(m_insertRecordButton, &QPushButton::clicked, this, &Cfhs_StartBatchDetect::insertRecordButton_clicked);
    //开始
    m_startButton = new QPushButton(this);
    m_startButton->setText(tr("开始"));
    connect(m_startButton, &QPushButton::clicked, this, &Cfhs_StartBatchDetect::startButton_clicked);
    //取消
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked, this, &Cfhs_StartBatchDetect::cancelButton_clicked);
    //给三个Button添加layout
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_insertRecordButton);
    buttonLayout->addWidget(m_startButton);
    buttonLayout->addWidget(m_cancelButton);
    //添加主界面layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addLayout(curRecordLayout);
    mainLayout->addLayout(alreadRecordLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(20);
    this->setLayout(mainLayout);
    //主界面设计
    this->setWindowTitle(tr("开始检测"));
    this->resize(400, 200);
}

Cfhs_StartBatchDetect::~Cfhs_StartBatchDetect()
{

}

void Cfhs_StartBatchDetect::setBatchInfo(const QString &nowBatch, const QStringList &listBatch)
{
    setBatchList(listBatch);
    m_curRecordLineEdit->setText(nowBatch);
    m_alreadyRecodeCombo->setCurrentText(nowBatch);
}

void Cfhs_StartBatchDetect::setBatchList(const QStringList &listBatch)
{
    m_alreadyRecodeCombo->clear();
    m_alreadyRecodeCombo->addItems(listBatch);
}

QString Cfhs_StartBatchDetect::getNowBatchName() const
{
    return m_curRecordLineEdit->text();
}

void Cfhs_StartBatchDetect::insertRecordButton_clicked()
{
    Cfhs_InsertRecord record(this);
    if(record.exec() == QDialog::Accepted)
    {
        //获取新的批次列表
        QString strAllBatch;
        QString strInfo;
        if(!m_logicInterface->GetAllBatchName(strAllBatch, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        QStringList listBatch = getListFromQString(strAllBatch);
        QString insertBatchName = record.getInsertBatchName();
        if(listBatch.contains(insertBatchName))
        {
            setBatchList(listBatch);
            m_alreadyRecodeCombo->setCurrentText(insertBatchName);
            QMessageBox::information(this, " ", tr("插入批次成功"));
        }
        else
            QMessageBox::warning(this, " ", tr("批次插入失败"));
    }
}

void Cfhs_StartBatchDetect::startButton_clicked()
{
    //设置当前批次
    QString nowBatch = m_alreadyRecodeCombo->currentText();
    if(nowBatch.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请选择执行批次"));
        return;
    }
    stConfig conf;
    QString strInfo;
    //获取配置信息
    if(!m_logicInterface->GetConfigInfo(conf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //设置配置信息
    conf.strNowBatchName = nowBatch;
    if(!m_logicInterface->SetConfigInfo(conf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    this->accept();
}

void Cfhs_StartBatchDetect::cancelButton_clicked()
{
    this->reject();
}
