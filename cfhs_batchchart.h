#ifndef CFHS_BATCHCHART_H
#define CFHS_BATCHCHART_H


#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_barchart.h>
#include "cfhs_base.h"
#include <QPointer>
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>


class Cfhs_BatchChart : public QwtPlot
{
    Q_OBJECT

public:
    Cfhs_BatchChart(QWidget *parent = nullptr);
    ~Cfhs_BatchChart();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

public slots:
    //---显示批次图表信息
    //输入
    //strHourName:小时名字（取值格式：#小时名1#小时名2...#小时名12）
    //strHourInput:每小时投入（取值格式：#小时投入1#小时投入2...#小时投入12）
    //strHourYield:每小时良率（取值格式：#小时良率1#小时良率2...#小时良率12）
    //strHourTotalYield:每小时累积良率（取值格式：#小时累积良率1#小时累积良率2...#小时累积良率12）
    void slot_ShowBatchChartInfo(const QString &strHourName, const QString &strHourInput,
                                 const QString &strHourYield, const QString &strHourTotalYield);

private slots:
    void on_actionExport_triggered();

private:
    QwtPlotBarChart *m_hourInput;
    QwtPlotCurve *m_hourYield;
    QwtPlotCurve *m_totalYield;
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_actionExport;
};

#endif // CFHS_BATCHCHART_H
