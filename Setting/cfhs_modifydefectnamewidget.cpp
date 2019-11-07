#include "cfhs_modifydefectnamewidget.h"
#include "cfhs_base.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>


Cfhs_ModifyDefectNameWidget::Cfhs_ModifyDefectNameWidget(const QString& curName, QWidget *parent)
    : QDialog(parent), m_defectName(curName)
{
    //当前缺陷名
    m_curNameLabel = new QLabel(this);
    m_curNameLabel->setText(tr("当前缺陷名："));
    m_curNameInfoLabel = new QLabel(this);
    m_curNameInfoLabel->setText(curName);
    QPointer<QHBoxLayout> curLayout = new QHBoxLayout();
    curLayout->addWidget(m_curNameLabel);
    curLayout->addWidget(m_curNameInfoLabel);
    //修改的缺陷名
    m_modifyNameLabel = new QLabel(this);
    m_modifyNameLabel->setText(tr("缺陷名修改为："));
    m_modifyNameLineEdit = new QLineEdit(this);
    QPointer<QHBoxLayout> modifyLayout = new QHBoxLayout();
    modifyLayout->addWidget(m_modifyNameLabel);
    modifyLayout->addWidget(m_modifyNameLineEdit);
    //确认button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_ModifyDefectNameWidget::onCommitButton_clicked);
    //取消button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_ModifyDefectNameWidget::onCancelButton_clicked);
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    //设置主界面layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addStretch();
    mainLayout->addLayout(curLayout);
    mainLayout->addLayout(modifyLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(30);
    this->setLayout(mainLayout);
    this->resize(400, 150);
    this->setWindowTitle(tr("修改缺陷名"));
}

Cfhs_ModifyDefectNameWidget::~Cfhs_ModifyDefectNameWidget()
{

}

QString Cfhs_ModifyDefectNameWidget::getDefectName()
{
    return m_defectName;
}

void Cfhs_ModifyDefectNameWidget::onCommitButton_clicked()
{
    QString name = m_modifyNameLineEdit->text();
    if(name.isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("请输入要修改的缺陷名"));
        return;
    }
    m_defectName = name;
    this->accept();
}

void Cfhs_ModifyDefectNameWidget::onCancelButton_clicked()
{
    this->reject();
}
