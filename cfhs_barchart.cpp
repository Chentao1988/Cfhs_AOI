#include "cfhs_barchart.h"
#include "qwt_scale_draw.h"
#include "qwt_plot_canvas.h"
#include "qwt_legend.h"
#include "qwt_plot_layout.h"
#include "qwt_plot_renderer.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_barchart.h"
#include "qwt_column_symbol.h"
#include "qwt_abstract_legend.h"
#include "qwt_symbol.h"
#include "qwt_plot_curve.h"
#include "qwt_painter.h"
#include <QRectF>
#include <QPointF>
#include <QRect>
#include <QDebug>
#include <QVector>
#include <QMenu>
#include <QAction>
#include <QColor>
#include <QCursor>
#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <QDate>


class DistroScaleDraw: public QwtScaleDraw
{
public:
    DistroScaleDraw(const QStringList& labels)
        : m_labels(labels)
    {
        //设置间距、中间刻度、主刻度
        setTickLength(QwtScaleDiv::MajorTick, 5);

        //不显示主轴线
        enableComponent(QwtScaleDraw::Backbone, false);

        //设置旋转角度、对齐方式
        setLabelAlignment(Qt::AlignVCenter);
    }

    virtual QwtText label(double value) const  //重绘坐标轴 刻度值
    {
        int index = static_cast<int>(value);
        QString text = "";
        if(index>=0 && index<m_labels.size())
            text = m_labels.at(index);

        return text;
    }

protected:
    //绘制时间轴
    virtual void drawLabel(QPainter *painter, double val) const
    {
        QFont font = this->label(val).font();
        QRectF rect = this->labelRect(font, val);

        QString text("");
        int index = static_cast<int>(val);
        if (index >= 0 && index < m_labels.size())
            text = m_labels[index];

        //获取文本的宽度
        QFontMetrics fm(font);
        int wid = fm.width(text);

        //获取左边的坐标
        int x = static_cast<int>((rect.width() - wid - 13) / 2);
        QPointF point = this->labelPosition(val);
        int left = static_cast<int>(point.x() - rect.width() / 2 + x);
        int xPos = left+6;
        int yPos = static_cast<int>(point.y());
        int width = static_cast<int>(rect.width()+6);
        int height = static_cast<int>(rect.height()+6);
        //qDebug()<<"xPos" << xPos <<",yPos" << yPos <<",width" <<width<<",height"<<height<<",text"<<text;
        painter->drawText(QRect(xPos, yPos, width, height), Qt::AlignLeft, text);
    }
private:
    QStringList m_labels;
};

class ColumnSymbol: public QwtColumnSymbol
{
public:
    ColumnSymbol(const int& value, const QColor& brushColor, const QColor& penColor=Qt::black)
        : QwtColumnSymbol()
    {
        m_value = value;
        m_brushColor = brushColor;
        m_penColor = penColor;
    }
#if 1
    //绘制柱形图
    virtual void draw(QPainter *p, const QwtColumnRect &rect) const
    {
        QRectF rect2 = rect.toRect();
        p->setBrush(m_brushColor);
        p->setPen(Qt::NoPen);
        p->drawRect(rect.toRect());
        p->setPen(QPen(m_penColor, 1));
        int top = static_cast<int>(rect2.top());
        int height = static_cast<int>(rect2.height());
        Qt::Alignment alignment;

        top -= 15;
        height += 15;
        alignment = Qt::AlignTop | Qt::AlignHCenter;
        int left = static_cast<int>(rect2.left());
        int width = static_cast<int>(rect2.width());
        p->drawText(QRect(left, top, width, height), alignment, QString::number(m_value));
    }
#endif

private:
    int m_value;
    QColor m_brushColor;
    QColor m_penColor;
};


class DistroChartItem: public QwtPlotBarChart
{
public:
    DistroChartItem(const QColor& brushColor, const QColor& penColor)
        :  QwtPlotBarChart()
    {
        m_brushColor = brushColor;
        m_penColor = penColor;
        this->setLegendIconSize(QSize(16,16));
    }
    DistroChartItem() : QwtPlotBarChart()
    {
        m_brushColor = QColor(Qt::blue);
        m_penColor = QColor(Qt::black);
        this->setLegendIconSize(QSize(16,16));
    }

    void setShowColor(const QColor& brushColor = Qt::blue, const QColor& penColor = Qt::black)
    {
        m_brushColor = brushColor;
        m_penColor = penColor;
    }

    void setData(QVector<double>& vector)
    {
        m_vector = vector;
    }
    void setSamplesData(QVector<double> samples)
    {
        //对整体除20倍处理，做到与良率的维度一致
        QVector<double> newSamples;
        foreach(double val, samples)
        {
            val /= 20.0;
            newSamples.append(val);
        }
        setSamples(newSamples);
    }

    //用于显示设置项
    virtual QwtColumnSymbol *specialSymbol(int index, const QPointF &) const
    {
        //自定义项符号线度、样式、背景色
        int value = 0;
        if (index >= 0 && index < m_vector.size())
            value = static_cast<int>(m_vector.at(index));

        ColumnSymbol *symbol = new ColumnSymbol(value, m_brushColor, m_penColor);

        return symbol;
    }

private:
    QVector<double> m_vector;
    QColor m_brushColor;
    QColor m_penColor;
};

class DistroSymbol : public QwtSymbol
{
public:
    DistroSymbol():QwtSymbol()
    {
        this->setStyle(QwtSymbol::Ellipse);
        this->setBrush(QBrush(Qt::yellow));
        this->setPen(QPen(Qt::red, 1));
        this->setSize(QSize(4,4));
    }
};

class DistroCurve : public QwtPlotCurve
{
public:
    DistroCurve():QwtPlotCurve()
    {
        this->setLegendAttribute(LegendShowLine);  //设置legend显示为线条
        setRenderHint(QwtPlotItem::RenderAntialiased, true);
    }
};

Cfhs_Barchart::Cfhs_Barchart(QWidget *parent):
    QwtPlot(parent)
{
    //设置标题、背景色
    setAutoFillBackground(true);
    setTitle(tr("良率/缺陷统计"));
    setMouseTracking(true);
    //设置网格
    m_grid = new QwtPlotGrid();
    m_grid->attach(this);
    //投入
    m_barChartItem = new DistroChartItem();
    m_barChartItem->setTitle(tr("投入/每小时"));
    m_barChartItem->attach(this);
    // 良率/每小时
    m_hourCurve = new DistroCurve();
    m_hourCurve->setTitle(tr("良率/每小时"));
    m_hourCurve->attach(this);
    // 累计良率/每小时
    m_totalCurve = new DistroCurve();
    m_totalCurve->setTitle(tr("累计良率/每小时"));
    m_totalCurve->attach(this);

    //设置值显示
    int xBottomAxisId = QwtPlot::xBottom;
    int yLeftAxisId = QwtPlot::yLeft;

    //设置X/Y轴标题、显示范围、轴线的最大间隔
    //setAxisTitle(xBottomAxisId, tr("生产时间"));
    QStringList listname;
    for(int i=0; i<12; i++)
        listname.append(QString::number(i+1));
    setAxisScale(xBottomAxisId, -1, 12);
    setAxisMaxMinor(xBottomAxisId, 1);  // 设置大刻度之间的小刻度数量
    setAxisMaxMajor(xBottomAxisId, 14); // 设置大刻度的数量,增加两个刻度使两侧的数据能够显示完全
    setAxisScaleDraw(xBottomAxisId, new DistroScaleDraw(listname));

    //setAxisTitle(yLeftAxisId, tr("产品良率"));
    setAxisScale(yLeftAxisId, 0, 100.0);
    setAxisMaxMinor(yLeftAxisId, 5);
    setAxisMaxMajor(yLeftAxisId, 10);
    setAxisScaleDraw(yLeftAxisId, new QwtScaleDraw());
    //enableAxis(yLeftAxisId, false); //设置不可用

    setAutoReplot(true);
    //Menu init
    m_menu = new QMenu(this);
    //Action init
    m_exportAction = new QAction();
    m_exportAction->setText(tr("导出"));
    connect(m_exportAction, &QAction::triggered,this,&Cfhs_Barchart::exportChart);
    m_menu->addAction(m_exportAction);
    //timer init
    m_timer = new QTimer(this);
    m_timer->setInterval(5000);
    //m_timer->start();
    connect(m_timer, &QTimer::timeout, this, &Cfhs_Barchart::timeout);
    //plotlayout
    QwtPlotLayout *layout = this->plotLayout();
    layout->setAlignCanvasToScales(true);
    this->setPlotLayout(layout);
}

Cfhs_Barchart::~Cfhs_Barchart()
{
    if(m_timer && m_timer->isActive())
        m_timer->stop();
}

void Cfhs_Barchart::setWidgetStyle()
{
    //主界面背景
    QString mainStyle = QString("color:#2193AB");
    this->setStyleSheet(mainStyle);
    QPalette palette;
    palette.setBrush(QPalette::Window, QColor("#000A19"));
    this->setPalette(palette);
    //设置画布边框宽度、样式、圆角大小、背景色
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth(1);
    canvas->setFrameStyle(QFrame::Box | QFrame::Sunken);
    canvas->setBorderRadius(6);
    QPalette canvasPalette(QColor("#000A19"));
    //canvasPalette.setColor(QPalette::WindowText, QColor("#2193AB"));
    canvas->setPalette(canvasPalette);
    setCanvas(canvas);
    //网格
    m_grid->setPen(QColor("#0C3497"), 1);
    //直方图
    m_barChartItem->setShowColor(QColor("#3954C9"), QColor("#24AFFF"));
    //小时良率曲线
    m_hourCurve->setPen(QColor("#24AFFF"), 2);
    //总良率曲线
    m_totalCurve->setPen(QColor("#6926C4"), 2);
    //重新设置legend
    insertLegend(new QwtLegend(), TopLegend);
    //重画
    replot();
}

void Cfhs_Barchart::clearContent()
{
    //产量重置
    QVector<double> samples;
    m_barChartItem->setSamplesData(samples);
    //良率重置
    QVector<QPointF> potSamples;
    m_hourCurve->setSamples(potSamples);
    //累计良率重置
    m_totalCurve->setSamples(potSamples);
}

void Cfhs_Barchart::setData(const QString &strHourName, const QString &strHourInput,
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
        double hourNameVal =0, hourInputVal=0;
        double hourYieldVal = 0.0, totalYieldVal = 0.0;
        int size = hourNameList.size();
        //
        QVector<double> inputPot;
        QPolygonF hourYieldPot;
        QPolygonF totalYieldPot;
        for(int i=0; i<size; i++)
        {
            hourNameVal = hourNameList.at(i).toDouble();
            hourInputVal = hourInputList.at(i).toDouble();
            hourYieldVal = hourYieldList.at(i).toDouble();
            totalYieldVal = totalYieldList.at(i).toDouble();
            inputPot.append(hourInputVal);
            hourYieldPot.append(QPointF(i, hourYieldVal));
            totalYieldPot.append(QPointF(i, totalYieldVal));
        }
        m_barChartItem->setData(inputPot);
        setDrawData(hourNameList, inputPot, hourYieldPot, totalYieldPot);
    }
    else
    {
        qDebug()<<"图表数据有误";
        return;
    }
}

void Cfhs_Barchart::setDrawData(const QStringList& name, const QVector<double>& inputSamples,
                 const QPolygonF& hourSamples, const QPolygonF& totalSamples)
{
#if 0
    //设置直显示
    int xBottomAxisId = QwtPlot::xBottom;
    int yLeftAxisId = QwtPlot::yLeft;

    //设置X/Y轴标题、显示范围、轴线的最大间隔
    //setAxisTitle(xBottomAxisId, tr("生产时间"));
    setAxisScale(xBottomAxisId, -1, 12);
    setAxisMaxMinor(xBottomAxisId, 1);  // 设置大刻度之间的小刻度数量
    setAxisMaxMajor(xBottomAxisId, 14); // 设置大刻度的数量,增加两个刻度使两侧的数据能够显示完全
    setAxisScaleDraw(xBottomAxisId, new DistroScaleDraw(name));

    //setAxisTitle(yLeftAxisId, tr("产品良率"));
    setAxisScale(yLeftAxisId, 0, 100.0);
    setAxisMaxMinor(yLeftAxisId, 5);
    setAxisMaxMajor(yLeftAxisId, 10);
    setAxisScaleDraw(yLeftAxisId, new QwtScaleDraw());
    //enableAxis(yLeftAxisId, false); //设置不可用
#else
    int xBottomAxisId = QwtPlot::xBottom;
    setAxisScaleDraw(xBottomAxisId, new DistroScaleDraw(name));
#endif
    //设置sample
    m_barChartItem->setSamplesData(inputSamples);
    m_hourCurve->setSamples(hourSamples);
    m_totalCurve->setSamples(totalSamples);
    //设置画布页边距
    plotLayout()->setCanvasMargin(5);

    //重新绘制
    replot();
}

void Cfhs_Barchart::timeout()
{
    QString strHourName = "";
    static int interVal = 1;
    const int num  = 12;
    for(int i=0; i<num; i++)
    {
        QString str = "";
        if(i<(num-1))
            str = QString("%1#").arg(i+interVal);
        else
            str = QString("%1").arg(i+interVal);
        strHourName.append(str);
    }
    QString strHourInput = "";
    for(int i=0; i<num; i++)
    {
        QString str = "";
        if(i<(num-1))
            str = QString("%1#").arg(5*(i+1)*20+12*interVal);
        else
            str = QString("%1").arg(5*(i+1)*20+12*interVal);
        strHourInput.append(str);
    }
    QString strHourYield = "" ;
    for(int i=0; i<num; i++)
    {
        QString str = "";
        if(i<(num-1))
            str = QString("%1#").arg(7*(i+1)+interVal);
        else
            str = QString("%1").arg(7*(i+1)+interVal);
        strHourYield.append(str);
    }
    QString strTotalYield = "";
    for(int i=0; i<num; i++)
    {
        QString str = "";
        if(i<(num-1))
            str = QString("%1#").arg((12-i)*(i+1)+interVal);
        else
            str = QString("%1").arg((12-i)*(i+1)+interVal);
        strTotalYield.append(str);
    }
    interVal++;
    if(interVal >=14)
        interVal = 1;
    setData(strHourName, strHourInput, strHourYield, strTotalYield);
}

//导出pdf
void Cfhs_Barchart::exportChart()
{
    QwtPlotRenderer renderer;
    QString filename = QString(tr("良率报表%1.pdf")).arg(QDate::currentDate().toString("yyyy-MM-dd"));
    if(renderer.exportTo(this, filename))
        QMessageBox::information(this, tr("提示"), tr("报表导出成功"));
}

void Cfhs_Barchart::contextMenuEvent(QContextMenuEvent *event)
{
    QCursor cursor;
    m_menu->exec(cursor.pos());

    QwtPlot::contextMenuEvent(event);
}
