#ifndef CFHS_STATIONSET_H
#define CFHS_STATIONSET_H

#include <QDialog>
#include <QMap>
#include <QList>
#include <QCloseEvent>
#include <QListWidgetItem>
#include "cfhs_base.h"
#include "Setting/cfhs_defectinfowidget.h"


//工位信息
typedef struct _StationInfo{
    int m_stationNo;  //工位号
    int m_port;  //工位端口
    int m_defectNum;  //缺陷数量
    bool m_isEnable;  //工位是否启用
    QString m_ip;  //工位Ip
    QMap<QString, QString> m_mapDefect;  //缺陷定义  key 缺陷名称  value 缺陷判定方式
    QMap<QString, QString> m_mapNg; //NG判定  key 缺陷名  value NG判定方式
    QStringList m_listFeature;  //缺陷特征值

    void init()
    {
        m_stationNo = 0;
        m_ip = "127.0.0.1";
        m_port = 0;
        m_defectNum = 0;
        m_isEnable = true;
        m_mapDefect.clear();
        m_mapNg.clear();
        m_listFeature.clear();
    }

}StationInfo;

//工位列表
typedef QList<StationInfo> StationInfoList;

namespace Ui {
class Cfhs_StationSet;
}

class Cfhs_StationSet : public QDialog
{
    Q_OBJECT

public:
    explicit Cfhs_StationSet(QWidget *parent = nullptr);
    ~Cfhs_StationSet();
    StationInfoList getStationList() const;
    void setStationList(const StationInfoList& list);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void init();
    //设置工位是否启用
    //isEnable  true/false
    void setStationEnable(const bool& isEnable);
    //刷新数据
    void updateData();
    //判断IP是否合法
    bool isIpValid(const QString& ip);
    //判定该工位的IP是否同其他工位重复
    bool isIpRepeated(const QString& ip);
    //判定该缺陷定义是否合法
    bool isDefectInfoValid(const QMap<QString, QString> &map);
    //设置缺陷特征
    void setFeatureList(const QStringList &list);
    //显示当前工位信息
    void showStationData();

private slots:
    //工位Ip设置
    void on_stationIpLineEdit_textEdited(const QString &curText);
    //工位是否启用
    void on_stationEnablePushButton_clicked();
    //工位端口设置
    void on_stationPortSpinBox_valueChanged(int value);
    //工位缺陷数量设置
    void on_defectNumSpinBox_valueChanged(int value);
    //选择工位
    void on_stationListWidget_currentRowChanged(int currentRow);

private:
    Ui::Cfhs_StationSet *ui;
    StationInfoList m_stationList;  //工位list
    StationInfo m_curStation;  //当前工位
    Cfhs_DefectInfoWidget *m_defectInfoWidget;  //缺陷定义
    Cfhs_DefectInfoTable *m_ngFilterWidget;  //NG判定
};

#endif // CFHS_STATIONSET_H
