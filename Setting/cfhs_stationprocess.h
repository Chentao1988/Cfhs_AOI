#ifndef CFHS_STATIONPROCESS_H
#define CFHS_STATIONPROCESS_H

#include <QListWidget>
#include <QList>
#include <QPointer>
#include "cfhs_stationsingletool.h"
#include "cfhs_global.h"

class QMouseEvent;
class QDropEvent;
class QDragMoveEvent;
class QDragEnterEvent;
class QContextMenuEvent;
class QMenu;
class QAction;


class Cfhs_StationProcess : public QListWidget
{
    Q_OBJECT
public:
    Cfhs_StationProcess(const stStation &station, QWidget *parent = nullptr);
    ~Cfhs_StationProcess();
    //设置工位
    void setStation(const stStation &station);
    //添加一个工具
    void addStationTool(StationTool& tool);
    //获取工位流程，Json格式
    QString getStationProcess() const;
    //获取工位号
    int getStationNo() const;
    //获取工位输出特征，json格式
    QString getStationFeature() const;
    //获取算法输出列表
    //输入
    //toolPosition  工具位置
    //输出
    //listEn  英文版
    //ListCns  简体中文版
    //listCnt  繁体中文版
    static bool getAlgorithmToolOutput(const QString &toolPosition, QStringList &listEn,
                                QStringList &listCns, QStringList &listCnt);

protected:
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    bool isAcceptedAddTool(const QString &toolPosition);
    void clearContent();

private slots:
    void slot_alterNameAction_triggered();
    void slot_alterTipAction_triggered();
    void slot_deleteAction_triggered();
    void slot_clearAction_triggered();

public slots:
    void slot_getRoiPoint(const int &stationNo, QMap<int,QList<QList<QPoint>>> &map);

private:
    QList<Cfhs_StationSingleTool*> m_listTool;
    Cfhs_StationSingleTool *m_actionItem;  //正在操作的item
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_alterNameAction;
    QPointer<QAction> m_alterTipAction;
    QPointer<QAction> m_deleteAction;
    QPointer<QAction> m_clearAction;
    bool m_isPressed; //左键按下
    QPoint m_startPos;  //鼠标拖动的起始位置
    int m_stationNo;
};

#endif // CFHS_STATIONPROCESS_H
