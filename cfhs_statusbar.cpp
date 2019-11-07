#include "cfhs_statusbar.h"
#include <QHBoxLayout>


Cfhs_StatusBar::Cfhs_StatusBar(QWidget *parent)
    : QStatusBar(parent)
{
    //当前日志
    m_curMsgLabel = new QLabel(this);
    //上一条日志
    m_prevMsgLabel = new QLabel(this);
    this->addWidget(m_prevMsgLabel, 1);
    this->addWidget(m_curMsgLabel, 1);
}

Cfhs_StatusBar::~Cfhs_StatusBar()
{

}

//---显示日志信息
//bNormal:true-正常信息，白色字体;false-异常信息，红色字体
//strInfo:执行信息
void Cfhs_StatusBar::ShowMessageInfo(const bool &bNormal, const QString& msg)
{
    //设置上一条日志
    m_prevMsgLabel->setText(m_curMsgLabel->text());
    m_prevMsgLabel->setStyleSheet(m_curMsgLabel->styleSheet());
    //设置当前日志
    QString style = (bNormal) ? "color:#2193AB" : "color:#D42E2E" ;
    m_curMsgLabel->setText(msg);
    m_curMsgLabel->setStyleSheet(style);
}
