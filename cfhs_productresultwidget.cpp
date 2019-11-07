#include "cfhs_productresultwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>


Cfhs_ProductResultWidget::Cfhs_ProductResultWidget(QWidget *parent)
    : QWidget(parent)
{
    //当前产品
    m_CurResultLabel = new QLabel(this);
    //前20产品
    for(int i=0; i<20; i++)
    {
        m_PrevResultLabel[i] = new QLabel(this);
    }
    //每10个产品添加layout
    QPointer<QHBoxLayout> layout1 = new QHBoxLayout();
    QPointer<QHBoxLayout> layout2 = new QHBoxLayout();
    for(int i=0; i<10; i++)
    {
        layout1->addWidget(m_PrevResultLabel[i], 0, Qt::AlignCenter);
        layout2->addWidget(m_PrevResultLabel[i+10], 0, Qt::AlignCenter);
    }
    //添加总layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addStretch();
    mainLayout->addWidget(m_CurResultLabel, 0, Qt::AlignCenter);
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->setSpacing(30);
    this->setLayout(mainLayout);
    //默认设置所有产品为Init状态
    resultInit();
}

Cfhs_ProductResultWidget::~Cfhs_ProductResultWidget()
{

}

void Cfhs_ProductResultWidget::resultInit()
{

    slot_ShowNowResult("Init");
    QString strResult20;
    for(int i=0; i<19; i++)
    {
        strResult20.append("Init#");
    }
    strResult20.append("Init");
    slot_Show20Result(strResult20);
}

void Cfhs_ProductResultWidget::showCurResult(QPointer<QLabel> label, const QString& strResult)
{
    QSize size = QSize(110, 110);
    if(label->size() != size)
        label->setFixedSize(size);
    QString style = "";
    if(strResult == "OK")
        style = "QLabel{border-image: url(:/ok_cur_show.png); font-size:50px;"
                "font-family:Mircosoft Yahei; font-weight:bold;"
                "color:#0077FF;}";
    else if(strResult == "NG")
        style = "QLabel{border-image: url(:/ng_cur_show.png); font-size:50px;"
                "font-family:Mircosoft Yahei; font-weight:bold;"
                "color:#0077FF;}";
    else
        style = "QLabel{border-image: url(:/init_cur_show.png); font-size:50px;"
                "font-family:Mircosoft Yahei; font-weight:bold;"
                "color:gray;}";

    label->setText(strResult);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(style);
}

void Cfhs_ProductResultWidget::showPrevResult(QPointer<QLabel> label, const QString& strResult)
{
    QSize size = QSize(40, 40);
    if(label->size() != size)
        label->setFixedSize(size);
    QString style = "";
    if(strResult == "OK")
        style = "QLabel{border-image: url(:/ok_prev_show.png)}";
    else if(strResult == "NG")
        style = "QLabel{border-image: url(:/ng_prev_show.png)}";
    else
        style = "QLabel{border-image: url(:/init_prev_show.png)}";

    label->setStyleSheet(style);
}

//显示当前产品结果(取值：OK代表良品,NG代表不良品)
void Cfhs_ProductResultWidget::slot_ShowNowResult(const QString& strResult)
{
    showCurResult(m_CurResultLabel, strResult);
}

//显示前20产品结果（取值格式：#第1个结果#第2个结果...#第20个结果）(取值：OK代表良品,NG代表不良品)
void Cfhs_ProductResultWidget::slot_Show20Result(const QString& strResult)
{
    QStringList resultList = strResult.split("#");
    int i = 0;
    foreach(QString result, resultList)
    {
        showPrevResult(m_PrevResultLabel[i], result);
        i++;
    }
}
