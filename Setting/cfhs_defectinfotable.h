#ifndef CFHS_DEFECTINFOTABLE_H
#define CFHS_DEFECTINFOTABLE_H

#include <QTableWidget>
#include <QPointer>

class QMenu;
class QAction;
class QContextMenuEvent;
class QMouseEvent;

//该类主要用于缺陷的显示和编辑
class Cfhs_DefectInfoTable : public QTableWidget
{
    Q_OBJECT

public:
    Cfhs_DefectInfoTable(const QString &functionName,
                         const QStringList &listFeature,
                         const bool &isEdited,
                         QWidget *parent = nullptr);
    ~Cfhs_DefectInfoTable();
    void setFeatureList(const QStringList &list);
    void setInfoMap(const QMap<QString, QString> &map);
    void getInfoMap(QMap<QString, QString> &map);
    //修改列标题
    void setHeadLabel(const int& column, const QString& name);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);


public slots:
    //设置缺陷定义方式
    void setDefectInfo(const QString& info);
    //设置缺陷名称
    void setDefectName(const QString& name);
    //删除一行
    void delOneRow(const QString& name);
    //添加一行
    //name 缺陷名称
    bool addOneRow(const QString& name);
    //修改缺陷名
    void editOneRow(const QString& oldName, const QString& newName);

private slots:
    void onModifyNameAction_triggered();
    void onDelRowAction_triggered();
    void onEditDefectAction_triggered();

signals:
    void sig_deleteOneRow(const QString &name);
    void sig_editOneRow(const QString &oldName, const QString &newName);

private:
    //缺陷名称，需要删除行和修改名称功能
    QPointer<QMenu> m_nameMenu;
    QPointer<QAction> m_modifyNameAction;
    QPointer<QAction> m_delRowAction;
    //缺陷判定方式，需要编辑功能
    QPointer<QMenu> m_defectMenu;
    QPointer<QAction> m_editDefectAction;
    int m_curRow;  //当前选择的行数
    int m_curCol;  //当前选择的列数
    QString m_functionName;  //功能名
    QStringList m_listFeature;   //缺陷特征列表
    bool m_isNameEdited;  //缺陷名称能否编辑
};

#endif // CFHS_DEFECTINFOTABLE_H
