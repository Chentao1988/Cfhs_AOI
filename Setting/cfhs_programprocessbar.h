#ifndef CFHS_PROGRAMPROCESSBAR_H
#define CFHS_PROGRAMPROCESSBAR_H

#include <QTabWidget>
#include <QMap>
#include "cfhs_stationprocess.h"


class Cfhs_ProgramProcessBar : public QTabWidget
{
    Q_OBJECT
public:
    Cfhs_ProgramProcessBar(QWidget *parent = nullptr);
    ~Cfhs_ProgramProcessBar();
    QMap<int, QString> getProgramProcessMap();

public slots:
    void setStationList(const QList<stStation> &list);
    void addStation(const stStation& station);
    void delStation(const int &stationNo);

public slots:
    void slot_getRoiPoint(QMap<int, QList<QList<QPoint> > > &map);

signals:
    void sig_sendRoiPoint(const int &stationNo, QMap<int,QList<QList<QPoint>>> &map);

private:
    //key  工位号
    //value  工位流程界面
    QMap<int, Cfhs_StationProcess*> m_mapStation;
};

#endif // CFHS_PROGRAMPROCESSBAR_H
