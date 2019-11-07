#include "cfhs_pwdmodifydialog.h"
#include "cfhs_global.h"
#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>


Cfhs_PwdModifyDialog::Cfhs_PwdModifyDialog(QWidget *parent) :
    QDialog(parent)
{
    //当前密码 init
    QPointer<QLabel> curLabel = new QLabel(tr("当前密码"), this);
    m_curPassword = new QLineEdit(this);
    m_curPassword->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    //新密码 init
    QPointer<QLabel> newLabel = new QLabel(tr("新密码"), this);
    m_newPassword = new QLineEdit(this);
    m_newPassword->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    //确认密码 init
    QPointer<QLabel> confirmLabel = new QLabel(tr("确认密码"), this);
    m_confirmPassword = new QLineEdit(this);
    m_confirmPassword->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    //添加GridLayout
    QPointer<QGridLayout> gridLayout = new QGridLayout();
    gridLayout->addWidget(curLabel, 0, 0, Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(m_curPassword, 0, 1);
    gridLayout->addWidget(newLabel, 1, 0, Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(m_newPassword, 1, 1);
    gridLayout->addWidget(confirmLabel, 2, 0, Qt::AlignRight|Qt::AlignVCenter);
    gridLayout->addWidget(m_confirmPassword, 2, 1);
    gridLayout->setVerticalSpacing(30);
    //添加 确认/取消 button
    m_cancelButton = new QPushButton(tr("取消"), this);
    m_commitButton = new QPushButton(tr("确认"), this);
    //给button添加layout
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addStretch();
    //添加界面Layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(gridLayout, 1);
    mainLayout->addLayout(buttonLayout, 0);
    this->setLayout(mainLayout);
    //设置窗口title
    this->setWindowTitle(tr("修改密码"));
    this->resize(400, 260);
    //信号槽连接
    connect(m_cancelButton, &QPushButton::clicked, this, &Cfhs_PwdModifyDialog::on_cancelButton_clicked);
    connect(m_commitButton, &QPushButton::clicked, this, &Cfhs_PwdModifyDialog::on_commitButton_clicked);
}

Cfhs_PwdModifyDialog::~Cfhs_PwdModifyDialog()
{

}

QString Cfhs_PwdModifyDialog::getPassword()
{
    return m_passwordModify;
}

void Cfhs_PwdModifyDialog::on_cancelButton_clicked()
{
    this->reject();
}

void Cfhs_PwdModifyDialog::on_commitButton_clicked()
{
    //当前密码判断
    QString curPd = m_curPassword->text();
    if(curPd.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请输入当前密码"));
        m_curPassword->setFocus();
        return;
    }
    //新密码判断
    QString newPd = m_newPassword->text();
    if(newPd.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请输入新密码"));
        m_newPassword->setFocus();
        return;
    }
    //确认密码判断
    QString confirmPd = m_confirmPassword->text();
    if(confirmPd.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请输入确认密码"));
        m_confirmPassword->setFocus();
        return;
    }
    //判断两次的密码是否一致
    if(newPd != confirmPd)
    {
        QMessageBox::warning(this, " ", tr("两次密码不一致，请重新输入"));
        m_newPassword->clear();
        m_confirmPassword->clear();
        m_newPassword->setFocus();
        return;
    }
    //判断新密码是否同当前密码一致
    if(newPd == curPd)
    {
        QMessageBox::warning(this, " ", tr("新的密码和当前密码一致，请修改"));
        m_newPassword->clear();
        m_confirmPassword->clear();
        m_newPassword->setFocus();
        return;
    }
    QString strInfo;
    if(!m_logicInterface->EditPWD(curPd, newPd, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        m_curPassword->clear();
        m_curPassword->setFocus();
        return;
    }
    m_passwordModify = newPd;
    this->accept();
}
