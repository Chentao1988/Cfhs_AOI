#include "cfhs_itodetectconfig.h"
#include "cfhs_algorithmtable.h"
#include "cfhs_global.h"
#include "cfhs_autoregionconfig.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>


const int m_paraNum = 8;

Cfhs_ItoDetectConfig::Cfhs_ItoDetectConfig(QWidget *parent)
    : QDialog(parent)
{
    //功能名
    m_nameLabel = new QLabel(this);
    m_nameLabel->setText(tr("ITO检测"));
    m_nameLabel->setStyleSheet(Cfhs_AutoRegionConfig::getToolLabelStyle());
    //参数表
    m_algoTable = new Cfhs_AlgorithmTable(this);
    m_algoTable->addOnePara(tr("缺陷长度"), tr("设定判定为缺陷的长度阈值"));
    m_algoTable->addOnePara(tr("分割阈值"), tr("网格与背景之间的灰度差大于该阈值将被分割出来"));
    m_algoTable->addOnePara(tr("上边缘"), tr("顶部边缘忽略宽度"));
    m_algoTable->addOnePara(tr("左边缘"), tr("左部边缘忽略宽度"));
    m_algoTable->addOnePara(tr("右边缘"), tr("右部边缘忽略宽度"));
    m_algoTable->addOnePara(tr("下边缘"), tr("下部边缘忽略宽度"));
    m_algoTable->addOnePara(tr("缺陷数量"), tr("设置检测的最大缺陷数量"));
    m_algoTable->addOnePara(tr("滤波强度"), tr("设置滤波强度值"));
    //commit button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_ItoDetectConfig::onCommitButton_clicked);
    //cancel button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_ItoDetectConfig::onCancelButton_clicked);
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

QString Cfhs_ItoDetectConfig::getParaConfig() const
{
    QMap<QString, QString> map;
    QString strInfo;
    getMapFromJson(m_strConfig, map, strInfo);
    //参数无效时返回默认值
    if(map.isEmpty())
        return getToolParaDefault();
    else
        return m_strConfig;
}

bool Cfhs_ItoDetectConfig::setParaConfig(const QString &strConfig)
{
    QString strInfo;
    QMap<QString, QString> map;
    getMapFromJson(strConfig, map, strInfo);
    if(map.isEmpty())
    {
        //参数无效，改为默认参数
        m_strConfig = getToolParaDefault();
        getMapFromJson(m_strConfig, map, strInfo);
    }
    QString strValue = "";
    int row = 0;
    QTableWidgetItem *item = nullptr;
    //缺陷长度
    if(map.contains("set_length"))
        strValue = map.take("set_length");
    else
        strValue = "40";
    row = getRowFromName("set_length");
    item = m_algoTable->item(row, 1);
    if(!item)
    {
        item = new QTableWidgetItem;
        m_algoTable->setItem(row, 1, item);
    }
    item->setText(strValue);
    //分割阈值
    if(map.contains("set_thresh"))
        strValue = map.take("set_thresh");
    else
        strValue = "15";
    row = getRowFromName("set_thresh");
    item = m_algoTable->item(row, 1);
    if(!item)
    {
        item = new QTableWidgetItem;
        m_algoTable->setItem(row, 1, item);
    }
    item->setText(strValue);
    //上边缘
    if(map.contains("set_top"))
        strValue = map.take("set_top");
    else
        strValue = "800";
    row = getRowFromName("set_top");
    item = m_algoTable->item(row, 1);
    if(!item)
    {
        item = new QTableWidgetItem;
        m_algoTable->setItem(row, 1, item);
    }
    item->setText(strValue);
    //左边缘
    if(map.contains("set_left"))
        strValue = map.take("set_left");
    else
        strValue = "1400";
    row = getRowFromName("set_left");
    item = m_algoTable->item(row, 1);
    if(!item)
    {
        item = new QTableWidgetItem;
        m_algoTable->setItem(row, 1, item);
    }
    item->setText(strValue);
    //右边源
    if(map.contains("set_right"))
        strValue = map.take("set_right");
    else
        strValue = "200";
    row = getRowFromName("set_right");
    item = m_algoTable->item(row, 1);
    if(!item)
    {
        item = new QTableWidgetItem;
        m_algoTable->setItem(row, 1, item);
    }
    item->setText(strValue);
    //下边缘
    if(map.contains("set_bottom"))
        strValue = map.take("set_bottom");
    else
        strValue = "800";
    row = getRowFromName("set_bottom");
    item = m_algoTable->item(row, 1);
    if(!item)
    {
        item = new QTableWidgetItem;
        m_algoTable->setItem(row, 1, item);
    }
    item->setText(strValue);
    //缺陷数量
    if(map.contains("set_limit"))
        strValue = map.take("set_limit");
    else
        strValue = "100";
    row = getRowFromName("set_limit");
    item = m_algoTable->item(row, 1);
    if(!item)
    {
        item = new QTableWidgetItem;
        m_algoTable->setItem(row, 1, item);
    }
    item->setText(strValue);
    //滤波强度
    if(map.contains("set_iter"))
        strValue = map.take("set_iter");
    else
        strValue = "8";
    row = getRowFromName("set_iter");
    item = m_algoTable->item(row, 1);
    if(!item)
    {
        item = new QTableWidgetItem;
        m_algoTable->setItem(row, 1, item);
    }
    item->setText(strValue);

    return true;
}

int Cfhs_ItoDetectConfig::getIndexFromName(const QString &name)
{
    int index = 0;
    if(name == "set_length")
        index = 1;
    else if(name == "set_thresh")
        index = 2;
    else if(name == "set_top")
        index = 3;
    else if(name == "set_left")
        index = 4;
    else if(name == "set_right")
        index = 5;
    else if(name == "set_bottom")
        index = 6;
    else if(name == "set_limit")
        index = 7;
    else if(name == "set_iter")
        index = 8;

    return index;
}

QString Cfhs_ItoDetectConfig::getNameFromIndex(const int &index)
{
    QString name = "";
    switch(index)
    {
    case 1:
        name = "set_length";
        break;
    case 2:
        name = "set_thresh";
        break;
    case 3:
        name = "set_top";
        break;
    case 4:
        name = "set_left";
        break;
    case 5:
        name = "set_right";
        break;
    case 6:
        name = "set_bottom";
        break;
    case 7:
        name = "set_limit";
        break;
    case 8:
        name = "set_iter";
        break;
    }

    return name;
}

int Cfhs_ItoDetectConfig::getRowFromName(const QString &name)
{
    return getIndexFromName(name) - 1;
}

void Cfhs_ItoDetectConfig::onCommitButton_clicked()
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

void Cfhs_ItoDetectConfig::onCancelButton_clicked()
{
    this->reject();
}
