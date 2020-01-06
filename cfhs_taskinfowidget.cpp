#include "cfhs_taskinfowidget.h"
#include "cfhs_base.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QButtonGroup>


Cfhs_TaskInfoWidget::Cfhs_TaskInfoWidget(QWidget *parent, const bool &isResultUsed)
    : QWidget(parent), m_isResultUsed(isResultUsed)
{
    //Group
    m_group = new QButtonGroup(this);
    //结果工位
    if(isResultUsed)
        m_reslutButton = addButton(tr("结果"));
    //分析工位
    m_analysisButton = addButton(tr("分析"));
    //添加layout
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addStretch(1);
    if(isResultUsed)
        layout->addWidget(m_reslutButton);
    layout->addWidget(m_analysisButton);
    layout->addStretch(2);
    layout->setSpacing(20);
    layout->setContentsMargins(15,0,15,0);
    this->setLayout(layout);
    m_curTaskIndex = AnalysisTask;
    m_curStationNum = -1;
}

Cfhs_TaskInfoWidget::~Cfhs_TaskInfoWidget()
{

}

void Cfhs_TaskInfoWidget::setTask(const int &stationNum)
{
    if(stationNum == m_curStationNum)
        return;
    //清空原有数据
    clearTask();
    QHBoxLayout *layout = static_cast<QHBoxLayout*>(this->layout());
    //如果新的工位不存在map中则添加
    int currentNum = m_mapButton.size();
    if(stationNum > currentNum)
    {
        for(int i=currentNum+1; i<=stationNum; i++)
        {
            QString name = QString(tr("工位%1")).arg(i);
            QPushButton *button = addButton(name);
            m_mapButton.insert(i, button);
        }
    }
    //重新显示工位
    for(int i=1; i<=stationNum; i++)
    {
        QPushButton *button = m_mapButton.value(i);
        button->setVisible(true);
        layout->addWidget(button);
    }
    layout->addStretch(2);
    //结果、分析工位状态置为Connect
    if(m_isResultUsed)
        setButtonStatus(m_reslutButton, Connect);
    setButtonStatus(m_analysisButton, Connect);

    //默认显示分析工位
    m_curTaskIndex = AnalysisTask;
    //m_curTaskIndex = 1;
    m_analysisButton->setChecked(true);

    m_curStationNum = stationNum;
}

int Cfhs_TaskInfoWidget::getCurrentTask() const
{
    return m_curTaskIndex;
}


void Cfhs_TaskInfoWidget::clearTask()
{
    QHBoxLayout *layout = static_cast<QHBoxLayout*>(this->layout());
    for(int i=1; i<=m_curStationNum; i++)
    {
        QPushButton *button = m_mapButton.value(i);
        button->setVisible(false);
        layout->removeWidget(button);
    }
    if(m_isResultUsed)
        layout->takeAt(3);
    else
        layout->takeAt(2);
}

QPushButton *Cfhs_TaskInfoWidget::addButton(const QString &name)
{
    QPushButton *button = new QPushButton(this);
    button->setText(name);
    button->setFixedSize(80, 30);
    button->setCheckable(true);
    setButtonStatus(button, Wait);
    connect(button, &QPushButton::clicked,
            this, &Cfhs_TaskInfoWidget::slot_currentTask_changed);
    //添加到group中
    m_group->addButton(button);
    return button;
}

void Cfhs_TaskInfoWidget::setButtonStatus(QPushButton *button, const StatusEnum &eStatus)
{
    QString style = "";
    switch(eStatus)
    {
    case Connect:
        style = QString("QPushButton{background:#0b6108;border-radius:6px;font-weight:bold;"
                        "border:none;font-size:16px; font-family:Mircosfot Yahei;color:#000A19;}"
                        "QPushButton:checked{border:2px solid red}");
        break;
    case Unconnect:
        style = QString("QPushButton{background:#910b0b;border-radius:6px;font-weight:bold;"
                        "border:none;font-size:16px; font-family:Mircosfot Yahei;color:#000A19;}"
                        "QPushButton:checked{border:2px solid red}");
        break;
    case Wait:
        style = QString("QPushButton{background:#bbbbbb;border-radius:6px;font-weight:bold;"
                        "border:none;font-size:16px; font-family:Mircosfot Yahei;color:#000A19;}"
                        "QPushButton:checked{border:2px solid red}");
        break;
    }

    button->setStyleSheet(style);
}

void Cfhs_TaskInfoWidget::slot_ShowStationStatus(int iStationNo, int iConnState)
{
    StatusEnum eStatus = static_cast<StatusEnum>(iConnState);
    QPushButton *button = m_mapButton.value(iStationNo);
    setButtonStatus(button, eStatus);
}

void Cfhs_TaskInfoWidget::slot_currentTask_changed()
{
    int index = 0;
    QPushButton *button = static_cast<QPushButton*>(sender());
    if(button == m_reslutButton)
        index = ResultTask;
    else if(button == m_analysisButton)
        index = AnalysisTask;
    else
        index = m_mapButton.key(button);

    if(index == m_curTaskIndex)
        return;

    if(index != 0)
    {
        m_curTaskIndex = index;
        emit sig_ShowCurrentTask(index);
    }
}
