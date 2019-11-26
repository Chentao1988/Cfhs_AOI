#include "cfhs_autoregionconfig_hjh.h"
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

const int m_paraNum = 3;

Cfhs_AutoRegionConfig_hjh::Cfhs_AutoRegionConfig_hjh(QWidget *parent)
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
    m_algoTable->addOnePara(tr("A区低阈值"), tr("相对于A区平均亮度下沉值"));
    m_algoTable->addOnePara(tr("A区高阈值"), tr("相对于A区平均亮度上浮值"));
    m_algoTable->addOnePara(tr("模板"), tr("0：调用已有模板  1：新建模板"));
    //commit button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_AutoRegionConfig_hjh::onCommitButton_clicked);
    //cancel button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_AutoRegionConfig_hjh::onCancelButton_clicked);
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

Cfhs_AutoRegionConfig_hjh::~Cfhs_AutoRegionConfig_hjh()
{

}

QString Cfhs_AutoRegionConfig_hjh::getShowName()
{
    QString name = tr("自动AA区2");

    return name;
}

QString Cfhs_AutoRegionConfig_hjh::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_AutoRegionConfig_hjh::getIconPath()
{
    QString path = ":/image_tool/auto_region_config.png";

    return path;
}

QString Cfhs_AutoRegionConfig_hjh::getToolName()
{
    return "AutoRegionConfig_hjh";
}

QString Cfhs_AutoRegionConfig_hjh::getParaConfig() const
{
    return m_strConfig;
}

bool Cfhs_AutoRegionConfig_hjh::setParaConfig(const QString &strConfig)
{
    QMap<QString, QString> map;
    QString strInfo;
    if(!getMapFromJson(strConfig, map, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    int row = 0;
    if(map.contains("aa_region_negative_hjh"))
    {
        QString strMin = map.value("aa_region_negative_hjh");
        row = getRowFromName("aa_region_negative_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strMin);
    }
    if(map.contains("aa_region_positive_hjh"))
    {
        QString strMax = map.value("aa_region_positive_hjh");
        row = getRowFromName("aa_region_positive_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strMax);
    }
    if(map.contains("new_model_hjh"))
    {
        QString strModel = map.value("new_model_hjh");
        row = getRowFromName("new_model_hjh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strModel);
    }

    return true;
}

int Cfhs_AutoRegionConfig_hjh::getIndexFromName(const QString &name)
{
    int index = 0;
    if(name == "aa_region_negative_hjh")
        index = 1;
    else if(name == "aa_region_positive_hjh")
        index = 2;
    else if(name == "new_model_hjh")
        index = 3;

    return index;
}

QString Cfhs_AutoRegionConfig_hjh::getNameFromIndex(const int &index)
{
    QString name = "";
    switch(index)
    {
    case 1:
        name = "aa_region_negative_hjh";
        break;
    case 2:
        name = "aa_region_positive_hjh";
        break;
    case 3:
        name = "new_model_hjh";
        break;
    }

    return name;
}

int Cfhs_AutoRegionConfig_hjh::getRowFromName(const QString &name)
{
    return getIndexFromName(name) -1;
}

void Cfhs_AutoRegionConfig_hjh::onCommitButton_clicked()
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

void Cfhs_AutoRegionConfig_hjh::onCancelButton_clicked()
{
    this->reject();
}

