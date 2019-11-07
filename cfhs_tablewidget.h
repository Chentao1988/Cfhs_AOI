#ifndef CFHS_TABLEWIDGET_H
#define CFHS_TABLEWIDGET_H

#include <QTableWidget>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QPointer>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QPoint>
#include "cfhs_base.h"



class Cfhs_TableWidget : public QTableWidget
{
    Q_OBJECT

public:
    //定义排序方式：SortAscend 升序，SortDescend 降序
    enum SortEnum{SortAscend=0, SortDescend};

    Cfhs_TableWidget(const QStringList& titleList, QWidget *parent = nullptr);
    Cfhs_TableWidget(QWidget *parent = nullptr);
    ~Cfhs_TableWidget();
    //设置横向标题栏
    //list  标题栏内容
    void setHeaderString(const QStringList& list);
    //添加数据
    //strData 数据 QString
    //split  分隔符
    void addData(const QString& strData, const QString& split="#");
    //清空表格
    void tableInit();
    //响应鼠标事件
    //hasMouseEvent  是否响应鼠标事件
    //column 响应事件的列数
    void setMouseEventInfo(const bool &hasMouseEvent, const int &column);
    //设置ContextEvent是否启用
    void setContextEventUsed(const bool &hasContextEvent);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    //窗口初始化
    //titleList  横向标题内容
    void init(const QStringList& titleList);
    //获取选中区域
    bool getSelectArea(int& topRow, int& leftColumn, int& bottomRow, int& rightColumn);
    //对数据进行排序
    //column 排序的当前列
    //eSort  排序的方式
    bool sortData(const int& column, const SortEnum& eSort);
    //获取完整数据表的实际列
    //column当前表格（有隐藏列）的当前列 -》映射到实际列
    int getRealColumn(const int& column);
    //判定该列是否需要隐藏
    bool isHidenColumn(const int& column);
    //将数据导出到Excel
    //filename  导出的文件名，包含路径
    //topRow  导出起始行
    //bottomRow  导出终止行
    bool exportToExcel(const QString& filename, const int& topRow, const int& bottomRow);
    //修改某行的数据
    //row 修改行
    //strData  修改数据
    //split  分割符
    void alterData( const int& row, const QString& strData, const QString& split="#");
    //获取某一数据在表格中的整行信息
    //value  当前数据值
    //curColumn  数据值所在的列数
    //split  数据分隔符
    QString getDataString(const float& value, const int& curColumn, const QString& split="#");

signals:
    void sig_showPoint(const QPoint &point);  //显示坐标

private slots:
    void on_sortAction_triggered();  //排序触发
    void on_hideAction_triggered();  //隐藏触发
    void on_showAction_triggered();  //显示触发,显示全部
    void on_exportAction_triggered(); //导出触发

private:
    QPointer<QMenu> m_mainMenu;
    QPointer<QAction> m_sortAction;
    QPointer<QAction> m_hideAction;
    QPointer<QAction> m_showAction;
    QPointer<QAction> m_exportAction;
    bool m_hasMouseEvent; //是否响应鼠标事件
    int m_mouseColumn;  //鼠标响应事件列数
    QStringList m_titleList;      //标题栏
    QStringList m_dataList;    //数据
    QList<int> m_hideColList;  //需要隐藏的列
    SortEnum m_eCurSort;  //当前排序方式
    bool m_hasContextEvent;  //是否响应Context事件
};

#endif // CFHS_TABLEWIDGET_H
