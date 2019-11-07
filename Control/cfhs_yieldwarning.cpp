#include "cfhs_yieldwarning.h"
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>


Cfhs_YieldWarning::Cfhs_YieldWarning(QWidget *parent)
    : QDialog(parent)
{
    //开始计算值
    const int countMin = 1, countMax = 10000;  //计算取值区间
    m_countLabel1 = new QLabel(this);
    m_countLabel1->setText(tr("产品运行"));
    m_countSpinbox = new QSpinBox(this);
    m_countSpinbox->setRange(countMin, countMax);
    m_countLabel2 = new QLabel(this);
    m_countLabel2->setText(tr("片后开始计算"));
    QPointer<QHBoxLayout> countLayout = new QHBoxLayout();
    countLayout->addWidget(m_countLabel1);
    countLayout->addWidget(m_countSpinbox);
    countLayout->addWidget(m_countLabel2);
    //良率提示
    const int promptMin = 0, promptMax = 100;  //良率提示取值区间
    m_promptLabel1 = new QLabel(this);
    m_promptLabel1->setText(tr("低于"));
    m_promptSpinbox = new QSpinBox(this);
    m_promptSpinbox->setRange(promptMin, promptMax);
    m_promptLabel2 = new QLabel(this);
    m_promptLabel2->setText(tr("%良率开始提示"));
    QPointer<QHBoxLayout> promptLayout = new QHBoxLayout();
    promptLayout->addWidget(m_promptLabel1);
    promptLayout->addWidget(m_promptSpinbox);
    promptLayout->addWidget(m_promptLabel2);
    //确定
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_YieldWarning::commitButton_clicked);
    //取消
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_YieldWarning::cancelButton_clicked);
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addStretch();
    //添加主界面layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addLayout(countLayout);
    mainLayout->addLayout(promptLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(50);
    mainLayout->setContentsMargins(10, 30, 10, 40);
    this->setLayout(mainLayout);
    //主界面设置
    this->setWindowTitle(tr("实时良率预警"));
    this->resize(500, 300);
}

Cfhs_YieldWarning::~Cfhs_YieldWarning()
{

}

void Cfhs_YieldWarning::setYieldValue(const int &startVal, const QString &strWarning)
{
    m_countSpinbox->setValue(startVal);
    int warnVal = strWarning.section("%", 0, 0).toInt();
    m_promptSpinbox->setValue(warnVal);
}

void Cfhs_YieldWarning::getYieldValue(int &startVal, QString &strWarning)
{
    startVal = m_countSpinbox->value();
    strWarning = QString("%1%").arg(m_promptSpinbox->value());
}

void Cfhs_YieldWarning::commitButton_clicked()
{
    this->accept();
}

void Cfhs_YieldWarning::cancelButton_clicked()
{
    this->reject();
}
