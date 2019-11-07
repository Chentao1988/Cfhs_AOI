#include "cfhs_messageinput.h"
#include "cfhs_base.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>


Cfhs_MessageInput::Cfhs_MessageInput(const QString& name, QWidget *parent, const QStringList &listAlready)
    : QDialog (parent), m_listAlready(listAlready)
{
    //项目
    m_nameLabel = new QLabel(this);
    m_nameLabel->setText(name);
    //输入框
    QString info = QString(tr("请输入%1")).arg(name);
    m_msgLineEdit = new QLineEdit(this);
    m_msgLineEdit->setPlaceholderText(info);
    //添加layout
    QPointer<QHBoxLayout> topLayout = new QHBoxLayout();
    topLayout->addWidget(m_nameLabel);
    topLayout->addWidget(m_msgLineEdit);
    //确定button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_MessageInput::onCommitButton_clicked);
    //取消button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_MessageInput::onCancelButton_clicked);
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
    this->setWindowTitle(info);
    this->resize(400, 200);
}

Cfhs_MessageInput::~Cfhs_MessageInput()
{

}

QString Cfhs_MessageInput::getMessage() const
{
    return m_strMessage;
}

void Cfhs_MessageInput::onCommitButton_clicked()
{
    m_strMessage = m_msgLineEdit->text();
    //检查内容是否为空
    if(m_strMessage.isEmpty())
    {
        QString info = m_msgLineEdit->placeholderText();
        QMessageBox::warning(this, " ", info);
        return;
    }
    //检查内容是否重复
    foreach(QString info, m_listAlready)
    {
        if(info == m_strMessage)
        {
            QString info = QString("%1已存在").arg(m_strMessage);
            QMessageBox::warning(this, " ", info);
            return;
        }
    }
    this->accept();
}

void Cfhs_MessageInput::onCancelButton_clicked()
{
    this->reject();
}
