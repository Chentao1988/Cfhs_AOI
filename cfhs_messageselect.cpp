#include "cfhs_messageselect.h"
#include "cfhs_base.h"
#include "cfhs_comboBox.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>


Cfhs_MessageSelect::Cfhs_MessageSelect(const QString& name, const QStringList& list, QWidget *parent)
    : QDialog (parent)
{
    //项目
    m_nameLabel = new QLabel(this);
    m_nameLabel->setText(name);
    //输入框
    m_msgCombo = new Cfhs_ComboBox(this);
    m_msgCombo->addItems(list);
    m_msgCombo->setCurrentIndex(0);
    //添加layout
    QPointer<QHBoxLayout> topLayout = new QHBoxLayout();
    topLayout->addWidget(m_nameLabel);
    topLayout->addWidget(m_msgCombo, 1);
    //确定button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_MessageSelect::onCommitButton_clicked);
    //取消button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_MessageSelect::onCancelButton_clicked);
    //添加button layout
    QPointer<QHBoxLayout> bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_cancelButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_commitButton);
    bottomLayout->addStretch();
    //添加主layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);
    //主界面设置
    QString title = QString(tr("请选择%1")).arg(name);
    this->setWindowTitle(title);
    this->resize(300, 200);
}

Cfhs_MessageSelect::~Cfhs_MessageSelect()
{

}

QString Cfhs_MessageSelect::getMessage() const
{
    return m_strMessage;
}

int Cfhs_MessageSelect::getCurrentIndex()
{
    int index = m_msgCombo->currentIndex();
    return index;
}

void Cfhs_MessageSelect::setCurrentIndex(const int &index)
{
    if(index>=0 && index<m_msgCombo->count())
        m_msgCombo->setCurrentIndex(index);
}

void Cfhs_MessageSelect::setCurrentText(const QString &text)
{
    m_msgCombo->setCurrentText(text);
}

void Cfhs_MessageSelect::onCommitButton_clicked()
{
    m_strMessage = m_msgCombo->currentText();
    if(m_strMessage.isEmpty())
    {
        QString info = this->windowTitle();
        QMessageBox::information(this, tr("提示"), info);
        return;
    }

    this->accept();
}

void Cfhs_MessageSelect::onCancelButton_clicked()
{
    this->reject();
}
