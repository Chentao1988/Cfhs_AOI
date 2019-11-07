#ifndef CFHS_TASKINFOWIDGET_H
#define CFHS_TASKINFOWIDGET_H

#include <QWidget>
#include <QMap>

class QPushButton;
class QButtonGroup;

class Cfhs_TaskInfoWidget : public QWidget
{
    Q_OBJECT
public:
    enum TaskEnum{ResultTask=-2, AnalysisTask};
    enum StatusEnum{Connect=1, Unconnect, Wait};
    explicit Cfhs_TaskInfoWidget(QWidget *parent = nullptr, const bool& isResultUsed = true);
    ~Cfhs_TaskInfoWidget();
    void setTask(const int &stationNum);
    int getCurrentTask() const;

private:
    void clearTask();
    QPushButton *addButton(const QString& name);
    void setButtonStatus(QPushButton *button, const StatusEnum &eStatus);

signals:
    void sig_ShowCurrentTask(const int &index);

public slots:
    void slot_ShowStationStatus(int iStationNo, int iConnState);

private slots:
    void slot_currentTask_changed();

private:
    int m_curTaskIndex;   //当前选定的显示项   结果: -2  分析：-1  工位：1~N
    QMap<int, QPushButton*> m_mapButton;
    bool m_isResultUsed;  //结果工位启用
    QPushButton *m_reslutButton;  //结果工位
    QPushButton *m_analysisButton; //分析工位
    int m_curStationNum;  //当前显示的工位数量
    QButtonGroup *m_group;
};

#endif // CFHS_TASKINFOWIDGET_H
