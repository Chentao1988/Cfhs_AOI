#ifndef CFHS_PROGRAMTOOLTREE_H
#define CFHS_PROGRAMTOOLTREE_H

#include <QTreeWidget>
#include <QMap>


class QTreeWidgetItem;
class QMouseEvent;
class QPoint;


class Cfhs_ProgramToolTree : public QTreeWidget
{
    Q_OBJECT

public:
    Cfhs_ProgramToolTree(QWidget *parent = nullptr);
    ~Cfhs_ProgramToolTree();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QTreeWidgetItem *getItem(const QString &showName, const QString &iconPath,
                             const QString &tip = "", const QString &toolName = "",
                             const int &column = 0);
    void init();

    static QString getRootIconPath();
    void addCameraModule();  //相机模块
    void addImagePartitionModule(); //图像分区模块
    void addAlgorithmModule(); //算法模块
private:
    bool m_isPressed;  //左键是否按下
    QPoint m_startPos;  //左键按下的起始位置
    //key  工具名
    //value  item
    QMap<QString, QTreeWidgetItem*> m_mapTool;
};

#endif // CFHS_PROGRAMTOOLTREE_H
