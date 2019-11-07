#include "cfhs_autoregionconfig.h"
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

const int m_paraNum = 2;

Cfhs_AutoRegionConfig::Cfhs_AutoRegionConfig(QWidget *parent)
    :QDialog (parent)
{
    //功能名
    m_nameLabel = new QLabel(this);
    m_nameLabel->setText(tr("分区参数"));
    QString style = QString("QLabel{background:#3954C9;border:none;font-size:16px;"
                            "font-family:Mircosoft Yahei; min-height:26px;"
                            "color:#2193AB;padding-left:15px;}");
    m_nameLabel->setStyleSheet(style);
    //参数表
    m_algoTable = new Cfhs_AlgorithmTable(this);
    m_algoTable->addOnePara(tr("边缘低阈值"), tr("a区提取亮度下限"));
    m_algoTable->addOnePara(tr("边缘高阈值"), tr("a区提取亮度上限"));
    //commit button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_AutoRegionConfig::onCommitButton_clicked);
    //cancel button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_AutoRegionConfig::onCancelButton_clicked);
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

Cfhs_AutoRegionConfig::~Cfhs_AutoRegionConfig()
{

}

QString Cfhs_AutoRegionConfig::getShowName()
{
    QString name = tr("自动AA区");

    return name;
}

QString Cfhs_AutoRegionConfig::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_AutoRegionConfig::getIconPath()
{
    QString path = ":/image_tool/auto_region_config.png";

    return path;
}

QString Cfhs_AutoRegionConfig::getToolName()
{
    return "AutoRegionConfig";
}

QString Cfhs_AutoRegionConfig::getParaConfig() const
{
    return m_strConfig;
}

bool Cfhs_AutoRegionConfig::setParaConfig(const QString &strConfig)
{
    QMap<QString, QString> map;
    QString strInfo;
    if(!getMapFromJson(strConfig, map, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    int row = 0;
    if(map.contains("aa_region_value_min"))
    {
        QString strMin = map.value("aa_region_value_min");
        row = getRowFromName("aa_region_value_min");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strMin);
    }
    if(map.contains("aa_region_value_max"))
    {
        QString strMax = map.value("aa_region_value_max");
        row = getRowFromName("aa_region_value_max");
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

int Cfhs_AutoRegionConfig::getIndexFromName(const QString &name)
{
    int index = 0;
    if(name == "aa_region_value_min")
        index = 1;
    else if(name == "aa_region_value_max")
        index = 2;

    return index;
}

QString Cfhs_AutoRegionConfig::getNameFromIndex(const int &index)
{
    QString name = "";
    switch(index)
    {
    case 1:
        name = "aa_region_value_min";
        break;
    case 2:
        name = "aa_region_value_max";
        break;
    }

    return name;
}

int Cfhs_AutoRegionConfig::getRowFromName(const QString &name)
{
    return getIndexFromName(name) -1;
}

void Cfhs_AutoRegionConfig::onCommitButton_clicked()
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

void Cfhs_AutoRegionConfig::onCancelButton_clicked()
{
    this->reject();
}
