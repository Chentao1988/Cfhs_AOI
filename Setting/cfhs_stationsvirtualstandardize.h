#ifndef CFHS_STATIONSVIRTUALSTANDARDIZE_H
#define CFHS_STATIONSVIRTUALSTANDARDIZE_H

#include <QDialog>
#include <QPointer>
#include "../cfhs_global.h"

class QLabel;
class QLineEdit;
class Cfhs_ComboBox;
class QTableWidget;
class QPushButton;
class QJsonObject;


//该类用于多工位投影标定设置
class Cfhs_StationsVirtualStandardize : public QDialog
{
    Q_OBJECT

public:
    Cfhs_StationsVirtualStandardize(QWidget *parent = nullptr);
    ~Cfhs_StationsVirtualStandardize();
    QString getStationStandardizeInfo() const;
    bool setStationStandardize(const QStringList &stationList, const QString &strStandardize);
    QString getNgFilterInfo() const;

private:
    void init();
    //获取该工位对应的缺陷名列表
    bool getStationDefectList(QStringList &listDefect, const int &stationNo);
    //设置投影工位1/2的列表
    void setStationList(const QStringList &stationList);
    //设置投影缺陷列表
    //defectList 缺陷列表，默认添加“无”选项
    //station  工位索引，取值1，2；1->投影工位1，对应表格纵向，2->投影工位2，对应表格横向
    void setStationDefectList(const QStringList& defectList, int station);
    QJsonObject getObject(const int &row, const int &col);
    //根据工位1和工位2的缺陷名，将新缺陷显示到相应位置
    bool setDefectText(const QString &strDefect1,
                       const QString &strDefect2,
                       const QString &newDefect);

private slots:
    void onStation1Combo_currentText_changed(const QString& text); //选择投影工位1
    void onStation2Combo_currentText_changed(const QString& text); //选择投影工位2
    void onStationSetButton_clicked();  //设定工位
    void onCommitButton_clicked();  //确定
    void onCancelButton_clicked();  //取消

private:
    //投影工位1
    QPointer<QLabel> m_station1Label;
    QPointer<Cfhs_ComboBox> m_station1Combo;
    QString m_station1Name;
    //投影工位2
    QPointer<QLabel> m_station2Label;
    QPointer<Cfhs_ComboBox> m_station2Combo;
    QString m_station2Name;
    //设定工位
    QPointer<QPushButton> m_stationSetButton;
    //标定信息表
    QPointer<QTableWidget> m_standardizeTable;
    //确定 button
    QPointer<QPushButton> m_commitButton;
    //取消 button
    QPointer<QPushButton> m_cancelButton;
    QStringList m_listStation1Title;
    QStringList m_listStation2Title;
    QString m_strStandardize; //工位投影信息
    QString m_strNg;  //NG筛选数据

};

#endif // CFHS_STATIONSVIRTUALSTANDARDIZE_H
