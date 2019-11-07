#include "cfhs_algorithmtable.h"
#include "../cfhs_base.h"
#include <QHeaderView>
#include <QMessageBox>


Cfhs_AlgorithmTable::Cfhs_AlgorithmTable(QWidget *parent)
    : QTableWidget (parent)
{
    //固定3列
    this->setColumnCount(3);
    //默认0行
    this->setRowCount(0);
    //设置列标题
    QStringList headList;
    headList.append(tr("参数名"));
    headList.append(tr("参数值"));
    headList.append(tr("参数说明"));
    this->setHorizontalHeaderLabels(headList);
    //设置表头不选中
    this->horizontalHeader()->setHighlightSections(false);
    //设置列宽
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

Cfhs_AlgorithmTable::~Cfhs_AlgorithmTable()
{

}

bool Cfhs_AlgorithmTable::setParaList(const QStringList &listName)
{
    if(listName.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("参数名列表不能为空"));
        return false;
    }
    //清空表格
    this->tableInit();
    //添加数据
    int size = listName.size();
    for(int i=0; i<size; i++)
    {
        QString strName =listName.at(i);
        if(!addOnePara(strName))
            return false;
    }

    return true;
}

bool Cfhs_AlgorithmTable::setParaMap(const QMap<int, QStringList> &map)
{
    if(map.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("参数名列表不能为空"));
        return false;
    }
    //清空表格
    this->tableInit();
    //添加数据
    QMap<int, QStringList>::const_iterator iter = map.begin();
    while(iter != map.end())
    {
        QString paraName = iter.value().at(0);
        QString paraInfo = iter.value().at(1);
        if(!addOnePara(paraName, paraInfo))
            return false;

        iter++;
    }

    return true;
}

bool Cfhs_AlgorithmTable::addOnePara(const QString &paraName)
{
    if(paraName.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("添加的参数名不能为空"));
        return false;
    }
    //检查该参数名是否已存在
    int rowCount = this->rowCount();
    for(int i=0; i<rowCount; i++)
    {
        QString curName = this->item(i, 0)->text();
        if(curName == paraName)
        {
            QString errorStr = QString(tr("参数名(%1)已存在，请重新添加")).arg(paraName);
            QMessageBox::warning(this, " ", errorStr);
            return false;
        }
    }
    //添加一行
    this->setRowCount(rowCount+1);
    //设置参数名
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(paraName);
    //设置为不可编辑
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    this->setItem(rowCount, 0, item);

    return true;
}

bool Cfhs_AlgorithmTable::addOnePara(const QString &paraName, const QString &paraInfo)
{
    if(paraName.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("添加的参数名不能为空"));
        return false;
    }
    if(paraInfo.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("添加的参数说明不能为空"));
        return false;
    }
    //检查该参数名是否已存在
    int rowCount = this->rowCount();
    for(int i=0; i<rowCount; i++)
    {
        QString curName = this->item(i, 0)->text();
        if(curName == paraName)
        {
            QString errorStr = QString(tr("参数名(%1)已存在，请重新添加")).arg(paraName);
            QMessageBox::warning(this, " ", errorStr);
            return false;
        }
    }
    //添加一行
    this->setRowCount(rowCount+1);
    //设置参数名
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(paraName);
    //设置为不可编辑
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    this->setItem(rowCount, 0, item);
    //设置参数说明
    QTableWidgetItem *infoItem = new QTableWidgetItem;
    infoItem->setText(paraInfo);
    //设置为不可编辑
    infoItem->setFlags(infoItem->flags() & (~Qt::ItemIsEditable));
    this->setItem(rowCount, 2, infoItem);
    return true;
}

bool Cfhs_AlgorithmTable::delOnePara(const QString &paraName)
{
    if(paraName.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("要删除的参数名不能为空"));
        return false;
    }
    //查找参数名对应的行
    int rowCount = this->rowCount();
    for(int i=0; i<rowCount; i++)
    {
        QString curName = this->item(i, 0)->text();
        if(curName == paraName)
        {
            //删除该行
            this->removeRow(i);
            return true;
        }
    }
    //未找到
    QString errorStr = QString(tr("参数表中未找到(%1)的数据")).arg(paraName);
    return false;
}

bool Cfhs_AlgorithmTable::getParaMap(QMap<int, QString> &map)
{
    map.clear();
    QString strInfo;
    int rowCount = this->rowCount();
    //检查数据是否合法
    for(int i=0; i<rowCount; i++)
    {
        QString strValue = "";
        QTableWidgetItem *item = this->item(i, 1);
        if(item)
            strValue = item->text();
        if(strValue.isEmpty())
        {
            strInfo = QString(tr("请设置第(%1)行的数据")).arg(i+1);
            QMessageBox::warning(this, " ", strInfo);
            return false;
        }
        //检查是否为int数据
        bool ok = false;
        strValue.toInt(&ok);
        if(!ok)
        {
            strInfo = QString(tr("第(%1)行的数据不是正数")).arg(i+1);
            QMessageBox::warning(this, " ", strInfo);
            return false;
        }
        map.insert(i+1, strValue);
    }

    return true;
}

void Cfhs_AlgorithmTable::tableInit()
{
    this->clearContents();
    this->setRowCount(0);
}
