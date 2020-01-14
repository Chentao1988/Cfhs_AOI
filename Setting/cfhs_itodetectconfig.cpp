#include "cfhs_itodetectconfig.h"
#include <QJsonDocument>
#include <QJsonObject>


Cfhs_ItoDetectConfig::Cfhs_ItoDetectConfig(QWidget *parent)
    : Cfhs_ToolBase(parent)
{
    //功能名
    setFunctionName(tr("ITO检测"));
    //参数表
    insertOnePara("set_length", tr("缺陷长度"), tr("设定判定为缺陷的长度阈值"));
    insertOnePara("set_thresh", tr("分割阈值"), tr("网格与背景之间的灰度差大于该阈值将被分割出来"));
    insertOnePara("set_top", tr("上边缘"), tr("顶部边缘忽略宽度"));
    insertOnePara("set_left", tr("左边缘"), tr("左部边缘忽略宽度"));
    insertOnePara("set_right", tr("右边缘"), tr("右部边缘忽略宽度"));
    insertOnePara("set_bottom", tr("下边缘"), tr("下部边缘忽略宽度"));
    insertOnePara("set_limit", tr("缺陷数量"), tr("设置检测的最大缺陷数量"));
    insertOnePara("set_iter", tr("滤波强度"), tr("设置滤波强度值"));
    //工具名
    this->setWindowTitle(getShowName());
    //窗口大小
    this->resize(600, 460);
}

Cfhs_ItoDetectConfig::~Cfhs_ItoDetectConfig()
{

}

QString Cfhs_ItoDetectConfig::getShowName()
{
    QString strName = tr("ITO检测");

    return strName;
}

QString Cfhs_ItoDetectConfig::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_ItoDetectConfig::getIconPath()
{
    QString path = ":/image_tool/ito_config.png";

    return path;
}

QString Cfhs_ItoDetectConfig::getToolPosition()
{
    QString position = "3-3";

    return position;
}

QString Cfhs_ItoDetectConfig::getToolParaDefault()
{
    QJsonObject obj;
    obj.insert("set_length", "40");
    obj.insert("set_thresh", "15");
    obj.insert("set_top", "800");
    obj.insert("set_left", "1400");
    obj.insert("set_right", "200");
    obj.insert("set_bottom", "800");
    obj.insert("set_limit", "100");
    obj.insert("set_iter", "8");
    QJsonDocument doc;
    doc.setObject(obj);

    QString json = QString(doc.toJson(QJsonDocument::Compact));
    return json;
}

QStringList Cfhs_ItoDetectConfig::getToolOutput(const LanguageEnum &language)
{
    QStringList list;
    switch(language)
    {
    case English:
        list.append("ITO_Coordinate1");
        list.append("ITO_Coordinate2");
        list.append("ITO_Length");
        break;
    case SimplifiedChinese:
        list.append("ITO坐标1");
        list.append("ITO坐标2");
        list.append("ITO线长度");
        break;
    case TraditionalChinese:
        list.append("ITO坐標1");
        list.append("ITO坐標2");
        list.append("ITO線長度");
        break;
    }

    return list;
}

QString Cfhs_ItoDetectConfig::getParaConfig()
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(m_strConfig, map, strInfo);
    if(map.isEmpty())
        return getToolParaDefault();
    else
        return m_strConfig;
}

bool Cfhs_ItoDetectConfig::setParaConfig(const QString &strConfig)
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(strConfig, map, strInfo);
    if(map.isEmpty())
    {
        //数据错误，使用默认数据
        QString strDefault = getToolParaDefault();
        getMapFromJson(strDefault, map, strInfo);
        m_strConfig = strDefault;
    }
    int row = 0;
    foreach(ParaInfo info, m_vectorPara)
    {
        if(map.contains(info.m_toolName))
        {
            QString strValue = map.value(info.m_toolName);
            row = info.m_index - 1;
            QTableWidgetItem *item = m_algoTable->item(row, 1);
            if(!item)
            {
                item = new QTableWidgetItem;
                m_algoTable->setItem(row, 1, item);
            }
            item->setText(strValue);
        }
    }

    return true;
}
