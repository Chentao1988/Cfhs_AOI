#include "cfhs_defectconfig_hjh.h"
#include "../cfhs_base.h"
#include "../cfhs_global.h"
#include "cfhs_algorithmtable.h"
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>

const int m_paraNum = 7;

Cfhs_DefectConfig_hjh::Cfhs_DefectConfig_hjh(QWidget *parent)
    :QDialog (parent)
{
    //功能名
    m_nameLabel = new QLabel(this);
    m_nameLabel->setText(tr("缺陷参数"));
    QString style = QString("QLabel{background:#3954C9;border:none;font-size:16px;"
                            "font-family:Mircosoft Yahei; min-height:26px;"
                            "color:#2193AB;padding-left:15px;}");
    m_nameLabel->setStyleSheet(style);
    //参数表
    m_algoTable = new Cfhs_AlgorithmTable(this);
    m_algoTable->addOnePara(tr("缺陷面积上限"), tr("单一瑕疵面积检测上限"));
    m_algoTable->addOnePara(tr("缺陷采集数量"), tr("瑕疵数目提取上限"));
    m_algoTable->addOnePara(tr("缺陷面积下限"), tr("单一瑕疵面积检测下限"));
    m_algoTable->addOnePara(tr("滤波宽"), tr("膨胀操作核宽"));
    m_algoTable->addOnePara(tr("滤波高"), tr("膨胀操作核高"));
    m_algoTable->addOnePara(tr("滤波低阈值"), tr("瑕疵检测可接受灰度下限"));
    m_algoTable->addOnePara(tr("滤波高阈值"), tr("瑕疵检测可接受灰度上限"));
    //commit button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_DefectConfig_hjh::onCommitButton_clicked);
    //cancel button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_DefectConfig_hjh::onCancelButton_clicked);
    //添加button layot
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addStretch();
    //添加主layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_nameLabel);
    mainLayout->addWidget(m_algoTable, 1);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10,10,10,20);
    this->setLayout(mainLayout);
    this->setWindowTitle(getShowName());
    this->resize(600, 400);
}

Cfhs_DefectConfig_hjh::~Cfhs_DefectConfig_hjh()
{

}

QString Cfhs_DefectConfig_hjh::getShowName()
{
    QString name = tr("瑕疵检测2");

    return name;
}

QString Cfhs_DefectConfig_hjh::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_DefectConfig_hjh::getIconPath()
{
    QString path = ":/image_tool/defect_config.png";

    return path;
}

QString Cfhs_DefectConfig_hjh::getToolName()
{
    return "DefectConfig_hjh";
}

QString Cfhs_DefectConfig_hjh::getParaConfig() const
{
    return m_strConfig;
}

bool Cfhs_DefectConfig_hjh::setParaConfig(const QString &strConfig)
{
    QMap<QString, QString> map;
    QString strInfo;
    if(!getMapFromJson(strConfig, map, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    int row = 0;
    if(map.contains("max_area_hjh"))
    {
        QString strMaxArea = map.value("max_area_hjh");
        row = getRowFromName("max_area_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strMaxArea);
    }

    if(map.contains("num_for_stop_inspection_hjh"))
    {
        QString strNum = map.value("num_for_stop_inspection_hjh");
        row = getRowFromName("num_for_stop_inspection_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strNum);
    }
    if(map.contains("min_area_hjh"))
    {
        QString strMinArea = map.value("min_area_hjh");
        row = getRowFromName("min_area_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strMinArea);
    }
    if(map.contains("exception_dilate_element_width_hjh"))
    {
        QString strWid = map.value("exception_dilate_element_width_hjh");
        row = getRowFromName("exception_dilate_element_width_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strWid);
    }
    if(map.contains("exception_dilate_element_height_hjh"))
    {
        QString strHei = map.value("exception_dilate_element_height_hjh");
        row = getRowFromName("exception_dilate_element_height_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strHei);
    }
    if(map.contains("grey_difference_negative_hjh"))
    {
        QString strMin = map.value("grey_difference_negative_hjh");
        row = getRowFromName("grey_difference_negative_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strMin);
    }
    if(map.contains("grey_difference_positive_hjh"))
    {
        QString strMax = map.value("grey_difference_positive_hjh");
        row = getRowFromName("grey_difference_positive_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strMax);
    }

    return true;
}

int Cfhs_DefectConfig_hjh::getIndexFromName(const QString &name)
{
    int index = 0;
    if(name == "max_area_hjh")
        index = 1;
    else if(name == "num_for_stop_inspection_hjh")
        index = 2;
    else if(name == "min_area_hjh")
        index = 3;
    else if(name == "exception_dilate_element_width_hjh")
        index = 4;
    else if(name == "exception_dilate_element_height_hjh")
        index = 5;
    else if(name == "grey_difference_negative_hjh")
        index = 6;
    else if(name == "grey_difference_positive_hjh")
        index = 7;

    return index;
}

QString Cfhs_DefectConfig_hjh::getNameFromIndex(const int &index)
{
    QString name = "";
    switch (index)
    {
    case 1:
        name = "max_area_hjh";
        break;
    case 2:
        name = "num_for_stop_inspection_hjh";
        break;
    case 3:
        name = "min_area_hjh";
        break;
    case 4:
        name = "exception_dilate_element_width_hjh";
        break;
    case 5:
        name = "exception_dilate_element_height_hjh";
        break;
    case 6:
        name = "grey_difference_negative_hjh";
        break;
    case 7:
        name = "grey_difference_positive_hjh";
        break;
    }

    return name;
}

int Cfhs_DefectConfig_hjh::getRowFromName(const QString &name)
{
    return getIndexFromName(name) - 1;
}

void Cfhs_DefectConfig_hjh::onCommitButton_clicked()
{
    QMap<int, QString> map;
    if(!m_algoTable->getParaMap(map))
        return;
    //转成Json格式
    QString strInfo;
    QJsonObject obj;
    if(map.size() != m_paraNum)
    {
        strInfo = QString("%1的参数个数不对").arg(getShowName());
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QMap<int, QString>::const_iterator iter = map.begin();
    while(iter != map.end())
    {
        QString name = getNameFromIndex(iter.key());
        obj.insert(name, iter.value());
        iter++;
    }
    QJsonDocument doc;
    doc.setObject(obj);
    m_strConfig = QString(doc.toJson(QJsonDocument::Compact));

    this->accept();
}

void Cfhs_DefectConfig_hjh::onCancelButton_clicked()
{
    this->reject();
}

