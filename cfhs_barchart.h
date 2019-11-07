#ifndef CFHS_BARCHART_H
#define CFHS_BARCHART_H

#include "qwt_plot.h"
#include <QVector>
#include <QPolygonF>
#include <QContextMenuEvent>
#include <QTimer>


#pragma execution_character_set("utf-8")

class DistroChartItem;
class DistroCurve;
class QMenu;
class QAction;
class QColor;
class QwtPlotGrid;

class Cfhs_Barchart: public QwtPlot
{
    Q_OBJECT

public:
    Cfhs_Barchart(QWidget *parent = nullptr);
    ~Cfhs_Barchart();
    void setWidgetStyle();
    void clearContent();

public slots:
    void setData(const QString &strHourName, const QString &strHourInput,
                 const QString &strHourYield, const QString &strHourTotalYield);
    void exportChart();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    void populate();
    void setDrawData(const QStringList& name, const QVector<double>& inputSamples,
                     const QPolygonF& hourSamples, const QPolygonF& totalSamples);
    void timeout();

private:
    DistroChartItem *m_barChartItem;
    DistroCurve *m_hourCurve;
    DistroCurve *m_totalCurve;
    QwtPlotGrid *m_grid;
    QMenu *m_menu;
    QAction *m_exportAction;
    QTimer *m_timer;
};

#endif // CFHS_BARCHART_H
