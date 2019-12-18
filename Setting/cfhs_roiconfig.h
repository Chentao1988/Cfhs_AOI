#ifndef CFHS_ROICONFIG_H
#define CFHS_ROICONFIG_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QPointF>

typedef struct _RoiShape{
    int Shape;  //1：圆形  2：矩形   3：多边形
    QList<QPointF> Points;  //坐标点

    void Init()
    {
        Shape = 0;
        Points.clear();
    }

}RoiShape;

class Cfhs_RoiConfig : public QObject
{
    Q_OBJECT

public:
    Cfhs_RoiConfig(QObject *parent = nullptr);
    ~Cfhs_RoiConfig();
    static QString getShowName();  //工具显示名
    static QString getToolTip();  //工具介绍
    static QString getIconPath(); //图标路径
    static QString getToolPosition(); //工具位置，2-1 前位：模块索引，后位：该模块中的工具索引
    bool setParaConfig(const QString& strConfig);
    QString getParaConfig();
    void setPointData(const RoiShape &data);
    const RoiShape getPointData();

private:
    RoiShape m_shape;
};

#endif // CFHS_ROICONFIG_H
