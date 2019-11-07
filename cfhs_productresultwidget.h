#ifndef CFHS_PRODUCTRESULTWIDGET_H
#define CFHS_PRODUCTRESULTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPointer>
#include "cfhs_base.h"


class Cfhs_ProductResultWidget : public QWidget
{
    Q_OBJECT

public:
    //该类主要用于在主界面显示产品结果，包括当前产品/前20产品结果
    //产品结果包括：OK代表良品 显示绿色；NG代表不良品 显示红色
    explicit Cfhs_ProductResultWidget(QWidget *parent = nullptr);
    ~Cfhs_ProductResultWidget();
    //结果显示重置
    void resultInit();

private:
    void showCurResult(QPointer<QLabel> label, const QString& strResult);
    void showPrevResult(QPointer<QLabel> label, const QString& strResult);

signals:

public slots:
    //显示当前产品结果(取值：OK代表良品,NG代表不良品)
    void slot_ShowNowResult(const QString& strResult);
    //显示前20产品结果（取值格式：#第1个结果#第2个结果...#第20个结果）(取值：OK代表良品,NG代表不良品)
    void slot_Show20Result(const QString& strResult);

private:
    QPointer<QLabel> m_CurResultLabel;
    QPointer<QLabel> m_PrevResultLabel[20];
};

#endif // CFHS_PRODUCTRESULTWIDGET_H
