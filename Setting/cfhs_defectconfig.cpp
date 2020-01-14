#include "cfhs_defectconfig.h"
#include <QJsonDocument>
#include <QJsonObject>


Cfhs_DefectConfig::Cfhs_DefectConfig(QWidget *parent)
    :Cfhs_ToolBase(parent)
{
    //参数表
    insertOnePara("defect_area_for_stop_inspection",
                  tr("大斑点面积"), tr("单一瑕疵面积检测上限"));
    insertOnePara("defect_num_for_stop_inspection",
                  tr("缺陷数量"), tr("瑕疵数目提取上限"));
    insertOnePara("defect_acceptable_minimum_area",
                  tr("小斑点面积"), tr("单一瑕疵面积检测下限"));
    insertOnePara("exception_dilate_element_width",
                  tr("滤波宽"), tr("膨胀操作核宽"));
    insertOnePara("exception_dilate_element_height",
                  tr("滤波高"), tr("膨胀操作核高"));
    insertOnePara("loop_stride_to_decide_stantard_grey_value",
                  tr("灰阶取值段数"), tr("可接受灰度值遍历步长"));
    insertOnePara("grey_difference_negative",
                  tr("滤波低阈值"), tr("瑕疵检测可接受灰度下限"));
    insertOnePara("grey_difference_positive",
                  tr("滤波高阈值"), tr("瑕疵检测可接受灰度上限"));
    //参数名
    setFunctionName(tr("缺陷参数"));
    //工具名
    setWindowTitle(getShowName());
    //窗体大小
    this->resize(600, 400);
}

Cfhs_DefectConfig::~Cfhs_DefectConfig()
{

}

QString Cfhs_DefectConfig::getShowName()
{
    QString name = tr("瑕疵检测1");

    return name;
}

QString Cfhs_DefectConfig::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_DefectConfig::getIconPath()
{
    QString path = ":/image_tool/defect_config.png";

    return path;
}

QString Cfhs_DefectConfig::getToolPosition()
{
    return "3-1";
}

QString Cfhs_DefectConfig::getToolParaDefault()
{
    QString strPara;
    QJsonObject obj;
    obj.insert("defect_area_for_stop_inspection", "50000");
    obj.insert("defect_num_for_stop_inspection", "5");
    obj.insert("defect_acceptable_minimum_area", "180");
    obj.insert("exception_dilate_element_width", "30");
    obj.insert("exception_dilate_element_height", "30");
    obj.insert("loop_stride_to_decide_stantard_grey_value", "100");
    obj.insert("grey_difference_negative", "30");
    obj.insert("grey_difference_positive", "30");
    QJsonDocument doc;
    doc.setObject(obj);

    strPara = QString(doc.toJson(QJsonDocument::Compact));
    return strPara;
}

QStringList Cfhs_DefectConfig::getToolOutput(const LanguageEnum &language)
{
    QStringList list;
    switch(language)
    {
    case English:
        list.append("DefectCoordinate");
        list.append("DefectArea1");
        list.append("DefectArea2");
        list.append("DefectWidth");
        list.append("DefectHeight");
        list.append("DefectGray");
        list.append("DefectGrayDifference");
        list.append("DefectRoundness");
        list.append("CircumferenceRatio");
        break;
    case SimplifiedChinese:
        list.append("缺陷坐标");
        list.append("缺陷面积1");
        list.append("缺陷面积2");
        list.append("缺陷宽度");
        list.append("缺陷长度");
        list.append("缺陷灰度");
        list.append("缺陷灰度差");
        list.append("缺陷圆度");
        list.append("圆周比率");
        break;
    case TraditionalChinese:
        list.append("缺陷坐標");
        list.append("缺陷面積1");
        list.append("缺陷面積2");
        list.append("缺陷寬度");
        list.append("缺陷長度");
        list.append("缺陷灰度");
        list.append("缺陷灰度差");
        list.append("缺陷圓度");
        list.append("圓周比率");
        break;
    }

    return list;
}

QString Cfhs_DefectConfig::getParaConfig()
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(m_strConfig, map, strInfo);
    if(map.isEmpty())
        return getToolParaDefault();
    else
        return m_strConfig;
}

bool Cfhs_DefectConfig::setParaConfig(const QString &strConfig)
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

