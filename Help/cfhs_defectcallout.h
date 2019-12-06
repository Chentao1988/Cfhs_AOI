#ifndef CFHS_DEFECTCALLOUT_H
#define CFHS_DEFECTCALLOUT_H

#include <QDialog>
#include <QPointer>
#include <QMap>
#include "cfhs_savepoint.h"

class QLabel;
class QPushButton;
class QTableWidget;
class Cfhs_Frame;
class QCloseEvent;
class QTableWidgetItem;


class Cfhs_DefectCallout : public QDialog
{
    Q_OBJECT
public:
    //dir  待处理的图片文件夹
    //parent  父窗口地址
    Cfhs_DefectCallout(const QString &dir, QWidget *parent = nullptr);
    ~Cfhs_DefectCallout();
    void setDirectory(const QString &dir);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void init();
    bool saveData();
    //读取配置文件
    //输入
    //path  文件路径
    //输出
    //list  配置参数，图形坐标
    bool readConfigFile(const QString &path);
    //保存配置文件
    //输入
    //path  文件路径
    //list  配置参数，图形坐标
    bool saveConfigFile(const QString &path);
    //更新缺陷坐标信息到内存
    void updateImagePoint(const int &index);
    //显示缺陷坐标信息到图片
    void showImagePoint(const int &index);


private slots:
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_commitButton_clicked();
    void on_cancelButton_clicked();
    void on_imageTable_currentCell_changed(int currentRow, int currentColumn,
                                           int previousRow, int previousColumn);
    void on_imageTable_cellDoubleClicked(int row, int column);

private:
    QPointer<QLabel> m_imageNameLabel;
    QPointer<QTableWidget> m_imageTable;
    QPointer<Cfhs_Frame> m_calloutFrame;
    QPointer<QPushButton> m_prevButton;
    QPointer<QPushButton> m_nextButton;
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
    QString m_directory;
    QMap<int, QList<NPoint>> m_mapPoints;  //缺陷图坐标
    QMap<int, QString> m_mapDefect; //缺陷名
    QPointer<Cfhs_SavePoint> m_pointWidget;
};

#endif // CFHS_DEFECTCALLOUT_H
