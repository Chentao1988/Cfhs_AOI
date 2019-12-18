#include "cfhs_defectinfotable.h"
#include "Setting/cfhs_defectjudgewidget.h"
#include "Setting/cfhs_modifydefectnamewidget.h"
#include "cfhs_base.h"
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QSize>
#include <QTableWidgetItem>
#include <QCursor>
#include <QPoint>
#include <QMessageBox>
#include <QDebug>
#include <QContextMenuEvent>
#include <QMouseEvent>



Cfhs_DefectInfoTable::Cfhs_DefectInfoTable(const QString &functionName,
                                           const QStringList &listFeature,
                                           const bool &isEdited,
                                           QWidget *parent)
    : QTableWidget (parent),m_functionName(functionName),
      m_listFeature(listFeature),m_isNameEdited(isEdited)
{

    QStringList headList;
    headList.append(tr("缺陷名称"));
    headList.append(tr("缺陷判定方式"));
    qDebug()<<"headlist" << headList;
    //默认设置行数为0（需要先设置行列，再添加标题和设置行列宽，否则无效）
    this->setRowCount(0);
    this->setColumnCount(2);
    //添加列标题
    this->setHorizontalHeaderLabels(headList);
    this->horizontalHeader()->setHighlightSections(false); //设置表头不选中
    this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //隐藏行标题
    this->verticalHeader()->setVisible(false);

    //设置列不可编辑
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setMouseTracking(true);
    //缺陷名称Menu
    m_nameMenu = new QMenu(this);
    //修改缺陷名
    m_modifyNameAction = new QAction();
    m_modifyNameAction->setText(tr("修改名称"));
    connect(m_modifyNameAction, &QAction::triggered,
            this, &Cfhs_DefectInfoTable::onModifyNameAction_triggered);
    //删除行
    m_delRowAction = new QAction();
    m_delRowAction->setText(tr("删除"));
    connect(m_delRowAction, &QAction::triggered,
            this, &Cfhs_DefectInfoTable::onDelRowAction_triggered);
    //添加到Menu中
    m_nameMenu->addAction(m_modifyNameAction);
    m_nameMenu->addAction(m_delRowAction);
    //缺陷判定方式
    m_defectMenu = new QMenu(this);
    //编辑缺陷判定方式
    m_editDefectAction = new QAction();
    m_editDefectAction->setText(tr("编辑"));
    connect(m_editDefectAction, &QAction::triggered,
            this, &Cfhs_DefectInfoTable::onEditDefectAction_triggered);
    //添加到Menu中
    m_defectMenu->addAction(m_editDefectAction);
    //value init
    m_curRow = -1;
    m_curCol = -1;
}

Cfhs_DefectInfoTable::~Cfhs_DefectInfoTable()
{

}

bool Cfhs_DefectInfoTable::addOneRow(const QString& name)
{
    int rowCount = this->rowCount();
    //检查该缺陷名是否是否存在
    for(int i=0; i<rowCount; i++)
    {
        if(this->item(i, 0))
        {
            if(this->item(i,0)->text() == name)
            {
                QString strInfo = QString(tr("第%1行已存在缺陷(%2)")).arg(i+1).arg(name);
                QMessageBox::warning(this, " ", strInfo);
                return false;
            }
        }
    }
    //添加一行
    this->setRowCount(rowCount+1);
    //设置缺陷名称
    QTableWidgetItem *item = this->item(rowCount, 0);
    if(item)
        item->setText(name);
    else
    {
        item = new QTableWidgetItem();
        item->setText(name);
        this->setItem(rowCount, 0, item);
    }

    return true;
}

void Cfhs_DefectInfoTable::editOneRow(const QString &oldName, const QString &newName)
{
    for(int i=0; i<this->rowCount(); i++)
    {
        if(this->item(i, 0))
        {
            if(this->item(i,0)->text() == oldName)
                this->item(i,0)->setText(newName);
        }
    }
}

void Cfhs_DefectInfoTable::setFeatureList(const QStringList &list)
{
    m_listFeature = list;
}

void Cfhs_DefectInfoTable::setInfoMap(const QMap<QString, QString> &map)
{
    int size = map.size();
    //设置行数
    this->setRowCount(size);
    //设置data
    QMap<QString, QString>::const_iterator iter = map.begin();
    int i = 0;
    while(iter != map.end())
    {
        QString strKey = iter.key();
        QString strVal = iter.value();
        //设置key  缺陷名
        QTableWidgetItem *itemKey = this->item(i, 0);
        if(itemKey)
            itemKey->setText(strKey);
        else
        {
            itemKey = new QTableWidgetItem();
            itemKey->setText(strKey);
            this->setItem(i, 0, itemKey);
        }
        //设置value  缺陷判定公式
        QTableWidgetItem *itemVal = this->item(i, 1);
        if(itemVal)
            itemVal->setText(strVal);
        else
        {
            itemVal = new QTableWidgetItem();
            itemVal->setText(strVal);
            this->setItem(i, 1, itemVal);
        }
        iter++;
        i++;
    }
}

void Cfhs_DefectInfoTable::getInfoMap(QMap<QString, QString> &map)
{
    //map重置
    map.clear();
    int rowCount = this->rowCount();
    for(int i=0; i<rowCount; i++)
    {
        QString strValue = "";
        QString strKey = "";
        QTableWidgetItem *itemKey = this->item(i, 0);
        QTableWidgetItem *itemVal = this->item(i, 1);
        if(!itemKey)
            strKey = "";
        else
            strKey = itemKey->text();
        if(!itemVal)
            strValue = "";
        else
            strValue = itemVal->text();
        map.insert(strKey, strValue);
    }
}

void Cfhs_DefectInfoTable::setHeadLabel(const int &column, const QString &name)
{
    int alterColumn = column - 1;
    if(alterColumn < 0 || alterColumn>=this->columnCount())
        return;
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(name);
    this->setHorizontalHeaderItem(alterColumn, item);
}

void Cfhs_DefectInfoTable::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    int curRow = this->currentRow();
    int curCol = this->currentColumn();
    if(curRow<0 || curCol<0)
        return;
    m_curRow = curRow;
    m_curCol = curCol;
    QCursor cursor;
    QPoint pos = cursor.pos();
    if(m_isNameEdited && curCol == 0)
    {
        //显示缺陷名称项
        m_nameMenu->exec(pos);
    }
    else if(curCol == 1)
    {
        //显示缺陷判定方式项
        m_defectMenu->exec(pos);
    }
}

void Cfhs_DefectInfoTable::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    int curRow = this->currentRow();
    int curCol = this->currentColumn();
    if(curRow<0 || curCol<0)
        return;
    m_curRow = curRow;
    m_curCol = curCol;
    QCursor cursor;
    QPoint pos = cursor.pos();
    if(m_isNameEdited && curCol == 0)
    {
        //显示缺陷名称项
        m_nameMenu->exec(pos);
    }
    else if(curCol == 1)
    {
        //显示缺陷判定方式项
        onEditDefectAction_triggered();
    }
}

void Cfhs_DefectInfoTable::delOneRow(const QString &name)
{
    for(int i=this->rowCount()-1; i>=0; i--)
    {
        if(this->item(i,0))
        {
            if(this->item(i,0)->text() == name)
                this->removeRow(i);
        }
    }
}

void Cfhs_DefectInfoTable::setDefectInfo(const QString& info)
{
    QTableWidgetItem *item = this->item(m_curRow, m_curCol);
    if(item)
        item->setText(info);
    else
    {
        item = new QTableWidgetItem();
        item->setText(info);
        this->setItem(m_curRow, m_curCol, item);
    }
}

void Cfhs_DefectInfoTable::setDefectName(const QString& name)
{
    QTableWidgetItem *item = this->item(m_curRow, m_curCol);
    if(item)
        item->setText(name);
    else
    {
        item = new QTableWidgetItem();
        item->setText(name);
        this->setItem(m_curRow, m_curCol, item);
    }
}

void Cfhs_DefectInfoTable::onModifyNameAction_triggered()
{
    QString oldName = this->item(m_curRow, m_curCol)->text();
    Cfhs_ModifyDefectNameWidget dialog(oldName, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        QString newName = dialog.getDefectName();
        setDefectName(newName);
        emit sig_editOneRow(oldName, newName);
    }
}

void Cfhs_DefectInfoTable::onDelRowAction_triggered()
{
    if(QMessageBox::Yes == QMessageBox::information(this
                                                    ,tr("提示")
                                                    ,tr("是否确认删除该行？")
                                                    ,QMessageBox::Yes
                                                    |QMessageBox::No))
    {
        QString name = this->item(m_curRow, 0)->text();
        delOneRow(name);
        emit sig_deleteOneRow(name);
    }
}

void Cfhs_DefectInfoTable::onEditDefectAction_triggered()
{
    QString strData = "";
    QTableWidgetItem *item = this->item(m_curRow, m_curCol);
    if(item)
        strData = item->text();
    Cfhs_DefectJudgeWidget dialog(m_functionName, m_listFeature, this);
    dialog.setDefectData(strData);
    connect(&dialog, &Cfhs_DefectJudgeWidget::sendDefectInfo,
            this, &Cfhs_DefectInfoTable::setDefectInfo);

    dialog.exec();
}
