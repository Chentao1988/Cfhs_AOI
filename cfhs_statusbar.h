#ifndef CFHS_STATUSBAR_H
#define CFHS_STATUSBAR_H

#include <QStatusBar>
#include <QLabel>
#include <QPointer>
#include "cfhs_base.h"


class Cfhs_StatusBar : public QStatusBar
{
public:
    Cfhs_StatusBar(QWidget *parent = nullptr);
    ~Cfhs_StatusBar();
    //---显示日志信息
    //bNormal:true-正常信息，白色字体;false-异常信息，红色字体
    //strInfo:执行信息
    void ShowMessageInfo(const bool &bNormal, const QString& msg);

private:
    QPointer<QLabel> m_curMsgLabel;
    QPointer<QLabel> m_prevMsgLabel;
};

#endif // CFHS_STATUSBAR_H
