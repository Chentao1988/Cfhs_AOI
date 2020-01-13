#include "cfhs_defectjudgetable.h"
#include "cfhs_base.h"
#include "cfhs_global.h"
#include "cfhs_combobox.h"
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QTableWidgetItem>
#include <QSize>
#include <QIcon>
#include <QSpinBox>
#include <QMessageBox>
#include <QCursor>
#include <QtSql/QSqlQuery>


Cfhs_DefectJudgeTable::Cfhs_DefectJudgeTable(const QStringList& nameList, QWidget* parent)
    : QTableWidget (parent), m_associatedNameList(nameList)
{
    //列标题
    QStringList headList;
    headList.append(tr("序号"));
    headList.append(tr("逻辑符号"));
    headList.append(tr("左括号"));
    headList.append(tr("关联项目"));
    headList.append(tr("关联操作符号"));
    headList.append(tr("关联数值(mm)"));
    headList.append(tr("右括号"));
    headList.append(tr("当前结果"));
    //默认设置行数为0
    this->setRowCount(0);
    int columnCount = headList.size();
    this->setColumnCount(columnCount);
    //添加列标题
    this->setHorizontalHeaderLabels(headList);
    this->horizontalHeader()->setHighlightSections(false); //设置表头不选中
    //设置列宽
    for(int i=0; i<columnCount; i++)
    {
        if(static_cast<ContentEnum>(i) == SerialNumber)
            this->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        else if(static_cast<ContentEnum>(i) == ResultInfo)
            this->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        else
            this->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    this->setColumnWidth(static_cast<int>(SerialNumber), 80);
    this->setColumnWidth(static_cast<int>(ResultInfo), 400);
    //隐藏行标题
    this->verticalHeader()->setVisible(false);
    //设置列不可编辑
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setMouseTracking(true);
    //设置Menu
    m_menu = new QMenu(this);
    //设置Action
    m_updateAction = new QAction();
    m_updateAction->setText(tr("刷新"));
    m_menu->addAction(m_updateAction);
    connect(m_updateAction, &QAction::triggered,
            this, &Cfhs_DefectJudgeTable::updateAction_triggered);
}

Cfhs_DefectJudgeTable::~Cfhs_DefectJudgeTable()
{

}

void Cfhs_DefectJudgeTable::addOneRow()
{
    int rowCount = this->rowCount();
    //添加一行
    this->setRowCount(rowCount + 1);
    //给该行添加控件
    addRowContent(rowCount);
    //刷新行序号
    updateRowsNum();
}

//插入一行
//index  插入的行索引
void Cfhs_DefectJudgeTable::insetOneRow(const int& index)
{
    this->insertRow(index);
    addRowContent(index);
    //刷新行序号
    updateRowsNum();
}

//删除一行
//row  要删除的行
void Cfhs_DefectJudgeTable::delOneRow(const int& row)
{
    //删除该行
    this->removeRow(row);
    //刷新行序号
    updateRowsNum();
}

//保存信息
//defectInfo 返回的缺陷判定方式
bool Cfhs_DefectJudgeTable::saveData(QString& defectInfo)
{
    //第一行的逻辑符号必须为空，其他行的逻辑符号不能为空
    int logic = static_cast<int>(LogicalSymbol);
    for(int i=0; i<this->rowCount(); i++)
    {
        Cfhs_ComboBox *logicCombo = static_cast<Cfhs_ComboBox*>(this->cellWidget(i, logic));
        if((i==0) && (logicCombo->currentIndex() != 0))
        {
            QMessageBox::warning(this, tr("错误"), tr("第1行的'逻辑符号'必须为空"));
            return false;
        }
        else if((i>0) && (logicCombo->currentIndex()==0))
        {
            QString errorStr = QString(tr("第%1行的'逻辑符号'不能为空")).arg(i+1);
            QMessageBox::warning(this, tr("错误"), errorStr);
            return false;
        }
    }
    //清空判定方式
    defectInfo.clear();
    //添加判定方式
    for(int i=0; i<this->rowCount(); i++)
    {
        QString info = getReslutDisplay(i);
        defectInfo.append(info);
    }
    if(defectInfo.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请添加缺陷判定方式"));
        return false;
    }
    //判定是否合法
    QString strInfo;
    if(!m_logicInterface->FormatCheck(m_curProgramName, m_curStationNo, defectInfo, strInfo))
    {
        QMessageBox::warning(this, " ", tr("判断语句不合法，请重新设置"));
        return false;
    }
    //刷新“当前结果”项
    updateAction_triggered();
    return true;
}

void Cfhs_DefectJudgeTable::addOneRowData(const QString &data)
{
    //添加一行
    addOneRow();
    int curRow = this->rowCount() - 1;
    int curCol = 0;
    QString strDefect = data;
    //逻辑符号
    QString strLogicalSymbol = "";
    if(strDefect.contains("&&"))
        strLogicalSymbol = "&&";
    else if(data.contains("||"))
        strLogicalSymbol = "||";
    curCol = static_cast<int>(LogicalSymbol);
    Cfhs_ComboBox *logicalSymbolCombo = static_cast<Cfhs_ComboBox*>(this->cellWidget(curRow, curCol));
    logicalSymbolCombo->setCurrentText(strLogicalSymbol);
    strDefect.remove(strLogicalSymbol);
    //左括号
    QString strLeftBrackets;
    if(strDefect.contains("(("))
        strLeftBrackets = "((";
    else
        strLeftBrackets = "(";
    curCol = static_cast<int>(LeftBrackets);
    Cfhs_ComboBox *comboLeftBrackets = static_cast<Cfhs_ComboBox*>(this->cellWidget(curRow, curCol));
    comboLeftBrackets->setCurrentText(strLeftBrackets);
    strDefect.remove(strLeftBrackets);
    //运算符
    QString strAssociatedSymbol = "";
    if(strDefect.contains(">="))
        strAssociatedSymbol = ">=";
    else if(strDefect.contains("<="))
        strAssociatedSymbol = "<=";
    else if(strDefect.contains(">"))
        strAssociatedSymbol = ">";
    else if(strDefect.contains("="))
        strAssociatedSymbol = "=";
    else if(strDefect.contains("<"))
        strAssociatedSymbol = "<";
    curCol = static_cast<int>(AssociatedSymbol);
    Cfhs_ComboBox *comboAssociatedSymbol = static_cast<Cfhs_ComboBox*>(this->cellWidget(curRow, curCol));
    comboAssociatedSymbol->setCurrentText(strAssociatedSymbol);
    //右括号
    QString strRightBrackets;
    if(strDefect.contains("))"))
        strRightBrackets = "))";
    else
        strRightBrackets = ")";
    curCol = static_cast<int>(RightBrackets);
    Cfhs_ComboBox *comboRightBrackets = static_cast<Cfhs_ComboBox*>(this->cellWidget(curRow, curCol));
    comboRightBrackets->setCurrentText(strRightBrackets);
    //参数名
    QString strAssociatedName = strDefect.section(strAssociatedSymbol, 0, 0);
    curCol = static_cast<int>(AssociatedName);
    Cfhs_ComboBox *comboAssociatedName = static_cast<Cfhs_ComboBox*>(this->cellWidget(curRow, curCol));
    comboAssociatedName->setCurrentText(strAssociatedName);
    //参数值
    QString strAssociatedVal = strDefect.remove(strAssociatedName)
            .remove(strAssociatedSymbol).remove(strRightBrackets);
    curCol = static_cast<int>(AssociatedVal);
    QSpinBox *spinAssociatedVal = static_cast<QSpinBox*>(this->cellWidget(curRow, curCol));
    spinAssociatedVal->setValue(strAssociatedVal.toInt());
    //结果
    QString strResultInfo = data;
    curCol = static_cast<int>(ResultInfo);
    QTableWidgetItem *itemResultInfo = this->item(curRow, curCol);
    if(!itemResultInfo)
    {
        itemResultInfo = new QTableWidgetItem;
        this->setItem(curRow, curCol, itemResultInfo);
    }
    itemResultInfo->setText(strResultInfo);
}

void Cfhs_DefectJudgeTable::setDefectData(const QString &strDefect)
{
    QStringList listDefect = getDefectList(strDefect);
    if(listDefect.isEmpty())
        return;
    foreach(QString strData, listDefect)
    {
        addOneRowData(strData);
    }
}

QStringList Cfhs_DefectJudgeTable::getDefectList(const QString &strDefect)
{
    QList<int> Number_List;
    int Nubmer_Find = 1;
    while (1) {
        Nubmer_Find = strDefect.indexOf("||",Nubmer_Find);
        if(Nubmer_Find == -1) {
            break;
        }
        Number_List<< Nubmer_Find;
        Nubmer_Find++;
    }
    Nubmer_Find = 1;
    while (1) {
        Nubmer_Find = strDefect.indexOf("&&",Nubmer_Find);
        if(Nubmer_Find == -1) {
            break;
        }
        Number_List<< Nubmer_Find;
        Nubmer_Find++;
    }
    QStringList String_List;
    if(!Number_List.empty()){
        std::sort(Number_List.begin(), Number_List.end());
        int Count;
        String_List<<strDefect.mid(0,Number_List[0]);
        for (Count = 1;Count<Number_List.size();Count++) {
            String_List<<strDefect.mid(Number_List[Count-1],Number_List[Count]-Number_List[Count-1]);
        }
        String_List<<strDefect.mid(Number_List[Count-1]);
    }
    else {
        String_List.append(strDefect);
    }

    return String_List;
}

void Cfhs_DefectJudgeTable::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    int curRow = this->currentRow();
    if(curRow < 0)
        return;
    ContentEnum curCol = static_cast<ContentEnum>(this->currentColumn());
    if(curCol != ResultInfo)
        return;
    QCursor cursor;
    m_menu->exec(cursor.pos());
}

//给该行添加控件
void Cfhs_DefectJudgeTable::addRowContent(const int& row)
{
    //添加序号
    QTableWidgetItem *rowItem = new QTableWidgetItem();
    rowItem->setText(QString::number(row+1));
    //序号列索引
    int rowIndex = static_cast<int>(SerialNumber);
    this->setItem(row, rowIndex, rowItem);
    //添加逻辑符号
    QStringList logicList;
    logicList.append(tr("  "));
    logicList.append(tr("&&"));
    logicList.append(tr("||"));
    Cfhs_ComboBox *logicCombo = new Cfhs_ComboBox();
    logicCombo->addItems(logicList);
    logicCombo->setCurrentIndex(0);
    //逻辑符号索引
    int logicIndex = static_cast<int>(LogicalSymbol);
    this->setCellWidget(row, logicIndex, logicCombo);
    //添加左括号
    QStringList leftBracketsList;
    leftBracketsList.append("(");
    leftBracketsList.append("((");
    Cfhs_ComboBox *leftCombo = new Cfhs_ComboBox();
    leftCombo->addItems(leftBracketsList);
    leftCombo->setCurrentIndex(0);
    int leftIndex = static_cast<int>(LeftBrackets);
    this->setCellWidget(row, leftIndex, leftCombo);
    //添加关联项目
    Cfhs_ComboBox *nameCombo = new Cfhs_ComboBox();
    nameCombo->addItems(m_associatedNameList);
    nameCombo->setCurrentIndex(0);
    int nameIndex = static_cast<int>(AssociatedName);
    this->setCellWidget(row, nameIndex, nameCombo);
    //添加关联操作符号
    QStringList operaList;
    operaList.append(">");
    operaList.append(">=");
    operaList.append("=");
    operaList.append("<");
    operaList.append("<=");
    Cfhs_ComboBox *operaCombo = new Cfhs_ComboBox();
    operaCombo->addItems(operaList);
    operaCombo->setCurrentIndex(0);
    int operaIndex = static_cast<int>(AssociatedSymbol);
    this->setCellWidget(row, operaIndex, operaCombo);
    //添加关联数值
    QSpinBox *valueSpin = new QSpinBox();
    valueSpin->setRange(0, 100000);
    valueSpin->setValue(0);
    int valueIndex = static_cast<int>(AssociatedVal);
    this->setCellWidget(row, valueIndex, valueSpin);
    //添加右括号
    QStringList rightList;
    rightList.append(")");
    rightList.append("))");
    Cfhs_ComboBox *rightCombo = new Cfhs_ComboBox();
    rightCombo->addItems(rightList);
    rightCombo->setCurrentIndex(0);
    int rightIndex = static_cast<int>(RightBrackets);
    this->setCellWidget(row, rightIndex, rightCombo);
}

void Cfhs_DefectJudgeTable::updateRowsNum()
{
    for(int i=0; i<this->rowCount(); i++)
    {
        QString info = QString::number(i+1);
        QTableWidgetItem *item = this->item(i, 0);
        if(item)
            item->setText(info);
        else
        {
            item = new QTableWidgetItem();
            item->setText(info);
            this->setItem(i, 0, item);
        }
    }

    this->update();
}

QString Cfhs_DefectJudgeTable::getReslutDisplay(const int& row)
{
    //逻辑符号
    int logic = static_cast<int>(LogicalSymbol);
    QString strLogic;
    Cfhs_ComboBox *logicCombo = static_cast<Cfhs_ComboBox*>(this->cellWidget(row, logic));
    if(logicCombo->currentIndex()==0)
        strLogic = "";
    else
        strLogic = logicCombo->currentText();
    //左括号
    int left = static_cast<int>(LeftBrackets);
    QString strLeft;
    Cfhs_ComboBox *leftCombo = static_cast<Cfhs_ComboBox*>(this->cellWidget(row, left));
    strLeft = leftCombo->currentText();
    //关联项目
    int name = static_cast<int>(AssociatedName);
    QString strName;
    Cfhs_ComboBox *nameCombo = static_cast<Cfhs_ComboBox*>(this->cellWidget(row, name));
    strName = nameCombo->currentText();
    //关联操作符号
    int opera = static_cast<int>(AssociatedSymbol);
    Cfhs_ComboBox *operaCombo = static_cast<Cfhs_ComboBox*>(this->cellWidget(row, opera));
    QString strOpera = operaCombo->currentText();
    //关联数值
    int value = static_cast<int>(AssociatedVal);
    QSpinBox *valueSpin = static_cast<QSpinBox*>(this->cellWidget(row, value));
    QString strVal = valueSpin->text();
    //右括号
    int right = static_cast<int>(RightBrackets);
    Cfhs_ComboBox *rightCombo = static_cast<Cfhs_ComboBox*>(this->cellWidget(row, right));
    QString strRight = rightCombo->currentText();
    //显示结果
    QString strResult = QString("%1%2%3%4%5%6")
            .arg(strLogic)
            .arg(strLeft)
            .arg(strName)
            .arg(strOpera)
            .arg(strVal)
            .arg(strRight);

    return strResult;
}

void Cfhs_DefectJudgeTable::updateAction_triggered()
{
    for(int curRow=0; curRow<this->rowCount(); curRow++)
    {
        QString resultText = getReslutDisplay(curRow);
        int curColumn = static_cast<int>(ResultInfo);
        QTableWidgetItem *item = this->item(curRow, curColumn);
        if(!item)
        {
            item = new QTableWidgetItem();
            this->setItem(curRow, curColumn, item);
        }

        item->setText(resultText);
    }
}
