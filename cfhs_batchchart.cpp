#include "cfhs_batchchart.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_renderer.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <QCursor>
#include <QHBoxLayout>
#include <QString>


Cfhs_BatchChart::Cfhs_BatchChart(QWidget *parent)
    : QwtPlot(parent)
{
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach(this);
    //条形图表样式
    QwtColumnSymbol *chartSymbol = new QwtColumnSymbol( QwtColumnSymbol::Box);
    chartSymbol->setLineWidth( 2 );
    chartSymbol->setFrameStyle(QwtColumnSymbol::Raised);
    chartSymbol->setPalette(QPalette(Qt::blue));
    //每小时产出
    m_hourInput = new QwtPlotBarChart();
    m_hourInput->setTitle(tr("产量/每小时"));
    m_hourInput->setSymbol(chartSymbol);
    m_hourInput->attach(this);
    //曲线样式
    QwtSymbol *hourSymbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 1 ), QSize( 6, 6 ));
    // 良率/每小时
    m_hourYield = new QwtPlotCurve();
    m_hourYield->setTitle(tr("良率/每小时"));
    m_hourYield->setPen( Qt::cyan, 4 );
    m_hourYield->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    m_hourYield->setSymbol(hourSymbol);
    m_hourYield->attach(this);
    QwtSymbol *totalSymbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 1 ), QSize( 6, 6 ));
    // 累计良率/每小时
    m_totalYield = new QwtPlotCurve();
    m_totalYield->setTitle(tr("累计良率/每小时"));
    m_totalYield->setPen( Qt::magenta, 4);
    m_totalYield->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    m_totalYield->setSymbol(totalSymbol);
    m_totalYield->attach(this);
    //导出
    m_menu = new QMenu(this);
    m_actionExport = new QAction(tr("保存"));
    m_menu->addAction(m_actionExport);
    connect(m_actionExport, &QAction::triggered, this, &Cfhs_BatchChart::on_actionExport_triggered);
    //layout
    QHBoxLayout *layout = new QHBoxLayout();
    this->setLayout(layout);

    this->setTitle(tr("良率/缺陷统计"));
    this->setCanvasBackground( Qt::white );
    this->setAutoFillBackground(true);
    this->insertLegend(new QwtLegend());
    this->setAutoReplot(true);
    this->setMouseTracking(true);

    this->show();

}

Cfhs_BatchChart::~Cfhs_BatchChart()
{

}

void Cfhs_BatchChart::contextMenuEvent(QContextMenuEvent *event)
{
    QCursor cursor;
    m_menu->exec(cursor.pos());

    QwtPlot::contextMenuEvent(event);
}

void Cfhs_BatchChart::slot_ShowBatchChartInfo(const QString &strHourName, const QString &strHourInput,
                             const QString &strHourYield, const QString &strHourTotalYield)
{
    QStringList hourNameList = strHourName.split("#");
    QStringList hourInputList = strHourInput.split("#");
    QStringList hourYieldList = strHourYield.split("#");
    QStringList totalYieldList = strHourTotalYield.split("#");
    if((!hourNameList.isEmpty())
            && (hourNameList.size() == hourInputList.size())
            && (hourNameList.size() == hourYieldList.size())
            && (hourNameList.size() == totalYieldList.size()))
    {
        //数据有效
        int hourNameVal =0, hourInputVal=0;
        double hourYieldVal = 0.0, totalYieldVal = 0.0;
        int size = hourNameList.size();
        //
        QVector<double> inputPot;
        QPolygonF hourYieldPot;
        QPolygonF totalYieldPot;
        for(int i=0; i<size; i++)
        {
            hourNameVal = hourNameList.at(i).toInt();
            hourInputVal = hourInputList.at(i).toInt();
            hourYieldVal = hourYieldList.at(i).toDouble();
            totalYieldVal = totalYieldList.at(i).toDouble();
            inputPot.append(hourInputVal);
            hourYieldPot.append(QPointF(hourNameVal, hourYieldVal));
            totalYieldPot.append(QPointF(hourNameVal, totalYieldVal));
        }
        m_hourInput->setSamples(inputPot);
        m_hourYield->setSamples(hourYieldPot);
        m_totalYield->setSamples(totalYieldPot);
    }
    else
    {
        qDebug()<<"图表数据有误";
        return;
    }
}

void Cfhs_BatchChart::on_actionExport_triggered()
{
    QwtPlotRenderer renderer;
    renderer.exportTo( this, tr("批次报表.pdf") );
}

