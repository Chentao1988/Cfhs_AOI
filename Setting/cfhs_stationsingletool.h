#ifndef CFHS_STATIONSINGLETOOL_H
#define CFHS_STATIONSINGLETOOL_H

#include "cfhs_cameraconfig.h"
#include "cfhs_autoregionconfig.h"
#include "cfhs_roiconfig.h"
#include "cfhs_defectconfig.h"
#include "cfhs_wavefilterconfig.h"
#include "cfhs_autoregionconfig_hjh.h"
#include "cfhs_defectconfig_hjh.h"
#include "cfhs_originalregionconfig.h"
#include "cfhs_itodetectconfig.h"
#include "cfhs_calibrationconfig.h"
#include "cfhs_lightfenceconfig.h"
#include <QListWidgetItem>

typedef struct _StationTool{
    int m_index;  //工具使用序号，从1开始
    int m_times;  //同类工具使用次数
    QString m_toolPosition;  //工具位置
    QString m_showName; //工具显示名
    QString m_toolPara;  //工具保存的参数，Json格式
}StationTool;

class Cfhs_StationSingleTool : public QListWidgetItem
{
public:
    Cfhs_StationSingleTool(const StationTool & tool, QListWidget *parent = nullptr);
    ~Cfhs_StationSingleTool();
    StationTool getStationTool();
    void setShowName(const QString &name);
    void setToolIndex(const int& index);
    void setToolPara(const QString &para);
    static QString getIconPath(const QString& toolPosition);
    static QString getToolTip(const QString& toolPosition);
    static QString getShowName(const QString& toolPosition);
    static QString getToolParaDefault(const QString& toolPosition);

private:
    void setToolPosition(const QString& strPosition);

private:
    StationTool m_stationTool;
};

#endif // CFHS_STATIONSINGLETOOL_H
