#include "cfhs_tablewidget.h"
#include "cfhs_base.h"
#include "cfhs_global.h"
#include <QCursor>
#include <QDebug>
#include <QTableWidgetSelectionRange>
#include <QMessageBox>
#include <QHeaderView>
#include <QAxObject>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QDir>
#include <QMap>
#include <QtAlgorithms>
#include <QFile>
#include <QVariant>
#include <QVariantList>


//排序方式 升序
static bool sortLessThan(float val1, float val2)
{
    return val1 <= val2;
}

//排序方式  降序
static bool sortGreaterThan(float val1, float val2)
{
    return val1 >= val2;
}
//排序列表
static QList<float> m_sortList;

Cfhs_TableWidget::Cfhs_TableWidget(const QStringList& titleList, QWidget *parent)
    : QTableWidget(parent)
{
    init(titleList);
}

Cfhs_TableWidget::Cfhs_TableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    init(QStringList());
}

Cfhs_TableWidget::~Cfhs_TableWidget()
{

}

void Cfhs_TableWidget::setHeaderString(const QStringList& list)
{
    int columnCount = list.size();
    this->setColumnCount(columnCount);
    this->setRowCount(0);
    this->setHorizontalHeaderLabels(list);
    m_titleList = list;
    //隐藏列list重置
    m_hideColList.clear();
}

void Cfhs_TableWidget::addData(const QString& strData, const QString& split)
{
    QStringList list = strData.split(split);
    //检测第一个字段是否为空
    if(list.at(0).isEmpty())
        return;
    int num = list.size();
    int rowCount = this->rowCount();
    //添加一行
    rowCount++;
    this->setRowCount(rowCount);
    //插入表的实际列
    int curCol = 0;
    for(int i=0; i<num; i++)
    {
        //判定是否是隐藏列，是的话不添加到表中
        if(isHidenColumn(i))
            continue;
        else
        {
            //插入的内容
            QString text = list.at(i);
            //添加item
            if(this->item(rowCount-1, curCol))
                this->item(rowCount-1, curCol)->setText(text);
            else
            {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setText(text);
                item->setTextAlignment(Qt::AlignCenter);
                this->setItem(rowCount-1, curCol, item);
            }
            //curCol + 1
            curCol++;
        }
    }
    //将该数据添加到Data list中
    m_dataList.append(strData);
}

void Cfhs_TableWidget::tableInit()
{
    this->clearContents();
    this->setRowCount(0);
}

void Cfhs_TableWidget::setMouseEventInfo(const bool &hasMouseEvent, const int &column)
{
    m_hasMouseEvent = hasMouseEvent;
    m_mouseColumn = column -1;
}

void Cfhs_TableWidget::setContextEventUsed(const bool &hasContextEvent)
{
    m_hasContextEvent  = hasContextEvent;
}

void Cfhs_TableWidget::alterData( const int& row, const QString& strData, const QString& split)
{
    QStringList list = strData.split(split);
    int num = list.size();
    //修改表的实际列
    int curCol = 0;
    for(int i=0; i<num; i++)
    {
        //判定是否是隐藏列，是的话不处理
        if(isHidenColumn(i))
            continue;
        else
        {
            //修改的内容
            QString text = list.at(i);
            //添加item
            if(this->item(row, curCol))
                this->item(row, curCol)->setText(text);
            else
            {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setText(text);
                item->setTextAlignment(Qt::AlignCenter);
                this->setItem(row, curCol, item);
            }
            //curCol + 1
            curCol++;
        }
    }
}

QString Cfhs_TableWidget::getDataString(const float& value, const int& curColumn, const QString& split)
{
    //获取当前列的标题
    QString title = this->horizontalHeaderItem(curColumn)->text();
    //找到该标题在完整表中的实际列
    int realColumn = 0;
    for(int i=0; i<m_titleList.size(); i++)
    {
        if(title == m_titleList.at(i))
        {
            realColumn = i;
            break;
        }
    }
    //所在行的数据
    QString strData = "";
    //将该数据转换成QString
    QString strValue = QString::number(double(value));
    //找到该数据在表中的实际行
    for(int i=0; i<m_dataList.size(); i++)
    {
        QStringList list = m_dataList.at(i).split(split);
        //如果数据处于该行的对应列则退出
        if(strValue == list.at(realColumn))
        {
            strData = m_dataList.takeAt(i);
            break;
        }
    }

    return strData;
}

void Cfhs_TableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if(m_hasContextEvent)
    {
        QCursor cursor;
        m_mainMenu->exec(cursor.pos());
    }

    QTableWidget::contextMenuEvent(event);
}

void Cfhs_TableWidget::mousePressEvent(QMouseEvent *event)
{
    if(!m_hasMouseEvent)
        return;
    int columnCount = this->columnCount();
    if(m_mouseColumn<0 || m_mouseColumn>=columnCount)
        return;
    //获取坐标点
    QTableWidgetItem *currentItem = this->itemAt(event->pos());
    if(currentItem)
    {
        int curRow = this->row(currentItem);
        QTableWidgetItem *dataItem = this->item(curRow, m_mouseColumn);
        if(dataItem)
        {
            QString text = dataItem->text();
            //不是坐标数据返回
            if(!text.contains(","))
                return;
            QPoint point;
            int xPos = text.section(",", 0, 0).toInt();
            int yPos = text.section(",", 1).toInt();
            point.setX(xPos);
            point.setY(yPos);
            emit sig_showPoint(point);
        }
    }

    QTableWidget::mousePressEvent(event);
}

void Cfhs_TableWidget::init(const QStringList& titleList)
{
    this->horizontalHeader()->setHighlightSections(false); //设置表头不选中
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->verticalHeader()->setVisible(false);
    this->setFocusPolicy(Qt::NoFocus);
    this->setMouseTracking(true);
    this->setHeaderString(titleList);
    m_mainMenu = new QMenu(this);
    m_sortAction = new QAction(tr("排序"), this);
    m_eCurSort = SortAscend; //默认升序
    m_hideAction = new QAction(tr("隐藏"), this);
    m_showAction = new QAction(tr("显示全部"), this);
    m_exportAction = new QAction(tr("导出"), this);
    m_mainMenu->addAction(m_sortAction);
    m_mainMenu->addAction(m_hideAction);
    m_mainMenu->addAction(m_showAction);
    m_mainMenu->addAction(m_exportAction);
    //value init
    m_hasMouseEvent = false;
    m_mouseColumn = -1;
    m_hasContextEvent = false;
    //连接信号槽
    connect(m_sortAction, &QAction::triggered,
            this, &Cfhs_TableWidget::on_sortAction_triggered);
    connect(m_hideAction, &QAction::triggered,
            this, &Cfhs_TableWidget::on_hideAction_triggered);
    connect(m_showAction, &QAction::triggered,
            this, &Cfhs_TableWidget::on_showAction_triggered);
    connect(m_exportAction, &QAction::triggered,
            this, &Cfhs_TableWidget::on_exportAction_triggered);
}

bool Cfhs_TableWidget::getSelectArea(int& topRow, int& leftColumn, int& bottomRow, int& rightColumn)
{
    QList<QTableWidgetSelectionRange> list = this->selectedRanges();
    if(list.isEmpty())
    {
        QMessageBox::information(this, " ", tr("请选择要操作的区域"));
        return false;
    }
    foreach(QTableWidgetSelectionRange range, this->selectedRanges())
    {
        topRow = range.topRow();
        leftColumn = range.leftColumn();
        bottomRow = range.bottomRow();
        rightColumn = range.rightColumn();
    }
    return true;
}

bool Cfhs_TableWidget::sortData(const int& column, const SortEnum& eSort)
{
    if(column == 0)
    {
        QString errorStr = QString(tr("[%1]不能进行排序，请选择其他列")).arg(m_titleList.at(column));
        QMessageBox::information(this, " ", errorStr);
        return false;
    }
    int rowCount = this->rowCount();   //行数
    //将该列数据添加到list中
    m_sortList.clear();
    for(int i=0; i<rowCount; i++)
    {
        float val = this->item(i, column)->text().toFloat();
        m_sortList.append(val);
    }
    //排序
    if(eSort == SortAscend)
        std::sort(m_sortList.begin(), m_sortList.end(), sortLessThan);
    else
        std::sort(m_sortList.begin(), m_sortList.end(), sortGreaterThan);


    qDebug()<<"list" << m_sortList;
    //将排序的数据重新显示
    QStringList datalist;
    datalist.clear();
    for(int i=0; i<rowCount; i++)
    {
        //数据值
        float value = m_sortList.at(i);
        //获取该值对应行的数据
        QString strData = getDataString(value, column);
        if(strData.isEmpty())
        {
            QMessageBox::warning(this, " ", tr("未找到数据对应行，请重新操作"));
            return false;
        }
        //修改该行数据
        alterData(i, strData);
        //添加到datalist中
        datalist.append(strData);
    }
    //更新m_dataList
    m_dataList = datalist;

    return true;
}

int Cfhs_TableWidget::getRealColumn(const int& column)
{
    //获取当前列的标题内容
    QTableWidgetItem *item = this->horizontalHeaderItem(column);
    QString title = item->text();
    //查找该内容在整表对应的列
    int realColumn = -1;
    for(int i=0; i<m_titleList.size(); i++)
    {
        QString str = m_titleList.at(i);
        if(str == title)
        {
            realColumn = i;
            break;
        }
    }

    return realColumn;
}

bool Cfhs_TableWidget::isHidenColumn(const int& column)
{
    bool isHided = false;
    for(int i=0; i<m_hideColList.size(); i++)
    {
        if(column == m_hideColList.at(i))
        {
            isHided = true;
            break;
        }
    }

    return isHided;
}

bool Cfhs_TableWidget::exportToExcel(const QString& filename, const int& topRow, const int& bottomRow)
{
    QAxObject *excel = new QAxObject;
    if (excel->setControl("Excel.Application")) //连接Excel控件
    {
        excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
        excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示

        QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
        workbooks->dynamicCall("Add");//新建一个工作簿
        QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
        QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);
        int i=0, j=0;
        int columnCount= this->columnCount(); //列数
        int rowCount = bottomRow-topRow+1; //实际行数
        QAxObject *cell,*col;
        //调整行高
        worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);

       QAxObject *range = nullptr;

        //列标题
        for(i=0;i<columnCount;i++)
        {
            QString columnName;
            columnName.append(QChar(i  + 'A'));
            columnName.append(":");
            columnName.append(QChar(i + 'A'));

            col = worksheet->querySubObject("Columns(const QString&)", columnName);
            col->setProperty("ColumnWidth", this->columnWidth(i)/6);

            cell=worksheet->querySubObject("Cells(int,int)", 1, i+1);
            columnName= this->horizontalHeaderItem(i)->text();
            cell->dynamicCall("SetValue(const QString&)", columnName);
            cell->querySubObject("Font")->setProperty("Bold", true);
            cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
            cell->setProperty("HorizontalAlignment", -4108);//xlCenter
            cell->setProperty("VerticalAlignment", -4108);//xlCenter
        }

#if 0
        //数据区
        for(i=topRow; i<=bottomRow; i++)
        {
            for (j=0; j<columnCount; j++)
            {
                worksheet->querySubObject("Cells(int,int)", i-topRow+2, j+1)->dynamicCall("SetValue(const QString&)", this->item(i,j)?this->item(i,j)->text():"");
            }
        }
#else
        //改成整体存储，提高保存效率
        QVariantList listData;
        for(i=topRow; i<=bottomRow; i++)
        {
            QVariantList listSingle;
            for (j=0; j<columnCount; j++)
            {
                QString text = this->item(i,j)?this->item(i,j)->text():"";
                listSingle.append(text);
            }
            listData.append(listSingle);
        }
        QVariant varData;
        varData.setValue(listData);
        QString vrange;
        vrange.append("A2:");
        vrange.append(columnCount - 1 + 'A');
        vrange.append(QString::number(rowCount + 1));
        range = worksheet->querySubObject("Range(const QString&)", vrange);
        range->dynamicCall("SetValue(const QVariant&)", varData);
#endif

        //画框线
        QString lrange;
        lrange.append("A1:");
        lrange.append(columnCount - 1 + 'A');
        lrange.append(QString::number(rowCount + 1));
        range = worksheet->querySubObject("Range(const QString&)", lrange);
        range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
        range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));

        //调整数据区行高
        QString rowsName;
        rowsName.append("2:");
        rowsName.append(QString::number(rowCount + 1));
        range = worksheet->querySubObject("Range(const QString&)", rowsName);
        range->setProperty("RowHeight", 20);

        workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(filename));//保存至fileName
        workbook->dynamicCall("Close()");//关闭工作簿
        excel->dynamicCall("Quit()");//关闭excel
        delete excel;
        excel=nullptr;
    }
    else
    {
        QMessageBox::warning(this, tr("错误"), tr("未能创建Excel对象，请安装Microsoft Excel"), QMessageBox::Apply);
        return false;
    }

    return true;
}

void Cfhs_TableWidget::on_sortAction_triggered()  //排序触发
{
    int topRow =0, leftColumn=0, bottomRow =0, rightColumn = 0;
    if(!getSelectArea(topRow, leftColumn, bottomRow, rightColumn))
        return;
    if(leftColumn != rightColumn)
    {
        QMessageBox::information(this, " ", tr("请选择单列操作"));
        return;
    }
    SortEnum sort = m_eCurSort;
    if(sortData(leftColumn, sort))
    {
        //切换到相反排序方式
        if(sort == SortAscend)
            sort = SortDescend;
        else
            sort = SortAscend;
        m_eCurSort = sort;
    }
}

void Cfhs_TableWidget::on_hideAction_triggered()  //隐藏触发
{
    int topRow =0, leftColumn=0, bottomRow =0, rightColumn = 0;
    if(!getSelectArea(topRow, leftColumn, bottomRow, rightColumn))
        return;
    for(int col = rightColumn; col>=leftColumn; col--)
    {
        if(col == 0)
        {
            QString errorStr = QString(tr("[%1]不能进行隐藏，请选择其他列")).arg(m_titleList.at(col));
            QMessageBox::information(this, " ", errorStr);
            return;
        }
        //将隐藏列的索引添加到隐藏list中
        int realColumn = getRealColumn(col);
        if(realColumn == -1)
        {
            QString errorStr = QString(tr("[%1列]不是有效列，请重新选择")).arg(col+1);
            QMessageBox::information(this, " ", errorStr);
            return;
        }
        m_hideColList.append(realColumn);
        //隐藏该列
        this->removeColumn(col);
    }
}

void Cfhs_TableWidget::on_showAction_triggered()  //显示触发,显示全部
{
    //重置表格，重新添加数据
    //添加标题栏
    QStringList titleList = m_titleList;
    setHeaderString(titleList);
    //添加数据
    QStringList dataList = m_dataList;
    m_dataList.clear();  //list重置
    foreach(QString data, dataList)
        this->addData(data);
}

void Cfhs_TableWidget::on_exportAction_triggered() //导出触发
{
    int topRow =0, leftColumn=0, bottomRow =0, rightColumn = 0;
    if(!getSelectArea(topRow, leftColumn, bottomRow, rightColumn))
        return;
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("导出数据"),
                                                    "",
                                                    tr("Excel文件(*.xls *.xlsx)"));
    if(filename.isEmpty())
        return;

    qDebug()<<"filename"<< filename;
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                       tr("等待"),
                                       tr("文件导出中，请等待..."),
                                       QMessageBox::NoButton,
                                       this);
    msg->show();
    Sleep(10);
    if(exportToExcel(filename, topRow, bottomRow))
    {
        //关闭提示窗口
        msg->hide();
        if(QMessageBox::question(this, tr("完成"), tr("文件已经导出，是否现在打开？"), QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(filename)));
    }
    //释放提示窗口
    delete msg;
    msg = nullptr;
}
