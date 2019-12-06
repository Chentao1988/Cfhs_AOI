#include "cfhs_programprocessbar.h"
#include "../cfhs_base.h"


Cfhs_ProgramProcessBar::Cfhs_ProgramProcessBar(QWidget *parent)
    : QTabWidget (parent)
{

}

Cfhs_ProgramProcessBar::~Cfhs_ProgramProcessBar()
{

}

QMap<int, QString> Cfhs_ProgramProcessBar::getProgramProcessMap()
{
    QMap<int, QString> mapProcess;
    if(m_mapStation.isEmpty())
        return mapProcess;
    QMap<int, Cfhs_StationProcess*>::const_iterator iter = m_mapStation.begin();
    while(iter != m_mapStation.end())
    {
        mapProcess.insert(iter.key(), iter.value()->getStationProcess());
        iter++;
    }

    return mapProcess;
}

QMap<int, QString> Cfhs_ProgramProcessBar::getStationFeatureMap()
{
    QMap<int, QString> mapFeature;
    if(m_mapStation.isEmpty())
        return mapFeature;
    QMap<int, Cfhs_StationProcess*>::const_iterator iter = m_mapStation.begin();
    while(iter != m_mapStation.end())
    {
        mapFeature.insert(iter.key(), iter.value()->getStationFeature());
        iter++;
    }

    return mapFeature;
}

void Cfhs_ProgramProcessBar::setStationList(const QList<stStation> &list)
{
    this->clear();
    for(int i= m_mapStation.size()-1; i>=0; i--)
    {
        Cfhs_StationProcess *station = m_mapStation.take(i+1);
        delete station;
        station = nullptr;
    }
    //重新添加工位
    if(list.isEmpty())
        return;
    for(int i=0; i<list.size(); i++)
    {
        addStation(list.at(i));
    }
}

void Cfhs_ProgramProcessBar::addStation(const stStation &station)
{
    QString name = QString(tr("工位%1")).arg(station.iStationNo);
    Cfhs_StationProcess *stationWidget = new Cfhs_StationProcess(station, this);
    connect(this, &Cfhs_ProgramProcessBar::sig_sendRoiPoint,
            stationWidget, &Cfhs_StationProcess::slot_getRoiPoint);
    this->addTab(stationWidget, name);
    m_mapStation.insert(station.iStationNo, stationWidget);
}

void Cfhs_ProgramProcessBar::delStation(const int &stationNo)
{
    Cfhs_StationProcess *station = m_mapStation.take(stationNo);
    if(station)
    {
        this->removeTab(stationNo-1);
        delete station;
        station = nullptr;
    }
}

void Cfhs_ProgramProcessBar::slot_getRoiPoint(QMap<int, QList<QList<QPoint>>> &map)
{
    int stationNo = this->currentIndex() + 1;
    emit sig_sendRoiPoint(stationNo, map);
}


