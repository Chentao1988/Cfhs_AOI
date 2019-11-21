#ifndef CFHS_PROGRAMCONFIG_H
#define CFHS_PROGRAMCONFIG_H

#include <QMainWindow>
#include <QPointer>
#include <QCloseEvent>
#include "Setting/cfhs_stationset.h"
#include "cfhs_base.h"
#include "Setting/cfhs_imagewindow.h"
#include "Setting/cfhs_stationsvirtualstandardize.h"
#include "Setting/cfhs_programtooltree.h"
#include "Setting/cfhs_programprocessbar.h"

class Cfhs_ShowStatusBar;

namespace Ui {
class Cfhs_ProgramConfig;
}

class Cfhs_ProgramConfig : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cfhs_ProgramConfig(QWidget *parent = nullptr);
    ~Cfhs_ProgramConfig();
    bool ReadProgram(const QString &programName);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void init();
    void setWindowStyle();
    bool isProgramNameValid();  //当前方案名是否有效
    bool isStationListValid();  //当前工位列表是否有效
    bool isMultipleStationValid(); //当前方案是否支持多工位功能
    void setCurProgramName(const QString& name);

private slots:
    //新建方案
    void on_newProgramAction_triggered();
    //加载方案
    void on_readProgramAction_triggered();
    //保存方案
    void on_saveProgramAction_triggered();
    //删除方案
    void on_deleteProgramAction_triggered();
    //添加工位
    void on_stationAddPushButton_clicked();
    //删除工位
    void on_stationDelPushButton_clicked();
    //工位设置
    void on_stationSetPushButton_clicked();
    //结果图像标定（分辨率设置）
    void on_resloutionSetPushButton_clicked();
    //工位判定逻辑
    void on_stationJudgePushButton_clicked();
    //NG筛选
    void on_ngFilterPushButton_clicked();
    //是否启用工位判定逻辑功能
    void on_stationsJudgeCheckBox_toggled(bool checked);
    //是否启用NG筛选功能
    void on_ngFilterCheckBox_toggled(bool checked);

private:
    Ui::Cfhs_ProgramConfig *ui;
    QPointer<Cfhs_StationSet> m_stationSetWidget;    //工位设置窗口
    QPointer<Cfhs_ImageWindow> m_imageWindow;    //显示产品图窗口
    QPointer<Cfhs_StationsVirtualStandardize> m_stationsStandardize;  //多工位投影标定
    QPointer<Cfhs_ProgramToolTree> m_toolTree;  //工具箱
    QPointer<Cfhs_ProgramProcessBar> m_processBar;  //流程栏
    stProgramme m_curProgram;  //当前方案
    QList<stStation> m_stationList; //工位列表
    bool m_isProgramSaved; //方案是否已保存
    QStringList m_featureList;  //缺陷特征列表
    QString m_strProgramName;  //当前方案名，方案配置界面不共用全局变量
    QPointer<Cfhs_ShowStatusBar> m_statusBar;  //日志
};

#endif // CFHS_PROGRAMCONFIG_H
