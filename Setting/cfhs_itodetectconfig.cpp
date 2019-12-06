#include "cfhs_itodetectconfig.h"
#include "cfhs_algorithmtable.h"
#include "cfhs_global.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>


const int m_paraNum = 2;

Cfhs_ItoDetectConfig::Cfhs_ItoDetectConfig(QWidget *parent)
    : QDialog(parent)
{
    //功能名
    m_nameLabel = new QLabel(this);
    m_nameLabel->setText(tr("ITO检测"));
    QString style = QString("QLabel{background:#3954C9;border:none;font-size:16px;"
                            "font-family:Mircosoft Yahei; min-height:26px;"
                            "color:#2193AB;padding-left:15px;}");
    m_nameLabel->setStyleSheet(style);
    //参数表
    m_algoTable = new Cfhs_AlgorithmTable(this);
    m_algoTable->addOnePara(tr("统计长度"), tr("统计平均阈值的线条长度"));
    m_algoTable->addOnePara(tr("判定阈值"), tr("灰度突变判定阈值"));
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
    this->resize(600, 300);
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
    obj.insert("detector_length", "80");
    obj.insert("detector_thresh", "10");
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
    if(map.contains("detector_length"))
    {
        strValue = map.take("detector_length");
        row = getRowFromName("detector_length");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strValue);
    }
    if(map.contains("detector_thresh"))
    {
        strValue = map.take("detector_thresh");
        row = getRowFromName("detector_thresh");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strValue);
    }

    return true;
}

int Cfhs_ItoDetectConfig::getIndexFromName(const QString &name)
{
    int index = 0;
    if(name == "detector_length")
        index = 1;
    else if(name == "detector_thresh")
        index = 2;

    return index;
}

QString Cfhs_ItoDetectConfig::getNameFromIndex(const int &index)
{
    QString name = "";
    if(index == 1)
        name = "detector_length";
    else if(index == 2)
        name = "detector_thresh";

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
