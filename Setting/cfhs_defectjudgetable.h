#ifndef CFHS_DEFECTJUDGETABLE_H
#define CFHS_DEFECTJUDGETABLE_H

#include <QTableWidget>
#include <QPointer>
#include <QEvent>
#include <QContextMenuEvent>


class QMenu;
class QAction;


//该类主要用于缺陷判定方式的编辑
class Cfhs_DefectJudgeTable : public QTableWidget
{
    Q_OBJECT

public:
    //表格每列对应显示的内容
    //SerialNumber 序号
    //LogicalSymbol 逻辑符号
    //LeftBrackets  左括号
    //AssociatedName  关联项目
    //AssociatedSymbol  关联操作符号
    //AssociatedVal  关联数值
    //RightBrackets  右括号
    //ResultInfo  显示结果
    enum ContentEnum{SerialNumber=0, LogicalSymbol, LeftBrackets, AssociatedName,
                    AssociatedSymbol, AssociatedVal, RightBrackets, ResultInfo};
    //nameList  关联项目list
    Cfhs_DefectJudgeTable(const QStringList& nameList, QWidget* parent = nullptr);
    ~Cfhs_DefectJudgeTable();
    void addOneRow();
    //插入一行
    //index  插入的行索引
    void insetOneRow(const int& index);
    //删除一行
    //row  要删除的行
    void delOneRow(const int& row);
    //保存信息
    //defectInfo 返回的缺陷判定方式
    bool saveData(QString& defectInfo);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    //给该行添加控件
    void addRowContent(const int& row);
    //更新所有行序号
    void updateRowsNum();
    //该行的显示结果
    QString getReslutDisplay(const int& row);

private slots:
    void updateAction_triggered();

private:
    QString m_defectInfo;   //缺陷判定方式
    QStringList m_associatedNameList;  //关联项目list
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_updateAction; //刷新结果

};

#endif // CFHS_DEFECTJUDGETABLE_H
