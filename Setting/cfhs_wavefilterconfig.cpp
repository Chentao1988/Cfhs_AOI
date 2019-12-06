#include "cfhs_wavefilterconfig.h"
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

const int m_paraNum = 5;

Cfhs_WaveFilterConfig::Cfhs_WaveFilterConfig(QWidget *parent)
    :QDialog (parent)
{
    //功能名
    m_nameLabel = new QLabel(this);
    m_nameLabel->setText(tr("滤波参数"));
    QString style = QString("QLabel{background:#3954C9;border:none;font-size:16px;"
                            "font-family:Mircosoft Yahei; min-height:26px;"
                            "color:#2193AB;padding-left:15px;}");
    m_nameLabel->setStyleSheet(style);
    //参数表
    m_algoTable = new Cfhs_AlgorithmTable(this);
    m_algoTable->addOnePara(tr("滤波宽"), tr("膨胀操作核宽"));
    m_algoTable->addOnePara(tr("滤波高"), tr("膨胀操作核高"));
    m_algoTable->addOnePara(tr("灰阶取值段数"), tr("可接受灰度值遍历步长"));
    m_algoTable->addOnePara(tr("滤波低阈值"), tr("瑕疵检测可接受灰度下限"));
    m_algoTable->addOnePara(tr("滤波高阈值"), tr("瑕疵检测可接受灰度上限"));
    //commit button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_WaveFilterConfig::onCommitButton_clicked);
    //cancel button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_WaveFilterConfig::onCancelButton_clicked);
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

Cfhs_WaveFilterConfig::~Cfhs_WaveFilterConfig()
{

}

QString Cfhs_WaveFilterConfig::getShowName()
{
    QString name = tr("膨胀滤波");

    return name;
}

QString Cfhs_WaveFilterConfig::getToolTip()
{
    QString tip = QString(tr("该工具主要用于设置算法(%1)的参数")).arg(getShowName());
    return tip;
}

QString Cfhs_WaveFilterConfig::getIconPath()
{
    QString path = ":/image_tool/wave_filter_config.png";

    return path;
}

QString Cfhs_WaveFilterConfig::getToolPosition()
{
    return "3-n";
}

QString Cfhs_WaveFilterConfig::getParaConfig() const
{
    return m_strConfig;
}

bool Cfhs_WaveFilterConfig::setParaConfig(const QString &strConfig)
{
    QMap<QString, QString> map;
    QString strInfo;
    if(!getMapFromJson(strConfig, map, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    int row = 0;
    if(map.contains("exception_dilate_element_width"))
    {
        QString strWid = map.value("exception_dilate_element_width");
        row = getRowFromName("exception_dilate_element_width");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strWid);
    }
    if(map.contains("exception_dilate_element_height"))
    {
        QString strHei = map.value("exception_dilate_element_height");
        row = getRowFromName("exception_dilate_element_height");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strHei);
    }
    if(map.contains("loop_stride_to_decide_stantard_grey_value"))
    {
        QString strGray = map.value("loop_stride_to_decide_stantard_grey_value");
        row = getRowFromName("loop_stride_to_decide_stantard_grey_value");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strGray);
    }
    if(map.contains("grey_difference_negative"))
    {
        QString strMin = map.value("grey_difference_negative");
        row = getRowFromName("grey_difference_negative");
        QTableWidgetItem *item = m_algoTable->item(row, 1);
        if(!item)
        {
            item = new QTableWidgetItem;
            m_algoTable->setItem(row, 1, item);
        }
        item->setText(strMin);
    }
    if(map.contains("grey_difference_positive"))
    {
        QString strMax = map.value("grey_difference_positive");
        row = getRowFromName("grey_difference_positive");
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

int Cfhs_WaveFilterConfig::getIndexFromName(const QString &name)
{
    int index = 0;
    if(name == "exception_dilate_element_width")
        index = 1;
    else if(name == "exception_dilate_element_height")
        index = 2;
    else if(name == "loop_stride_to_decide_stantard_grey_value")
        index = 3;
    else if(name == "grey_difference_negative")
        index = 4;
    else if(name == "grey_difference_positive")
        index = 5;

    return index;
}

QString Cfhs_WaveFilterConfig::getNameFromIndex(const int &index)
{
    QString name = "";
    switch (index)
    {
    case 1:
        name = "exception_dilate_element_width";
        break;
    case 2:
        name = "exception_dilate_element_height";
        break;
    case 3:
        name = "loop_stride_to_decide_stantard_grey_value";
        break;
    case 4:
        name = "grey_difference_negative";
        break;
    case 5:
        name = "grey_difference_positive";
        break;
    }

    return name;
}

int Cfhs_WaveFilterConfig::getRowFromName(const QString &name)
{
    int row = getIndexFromName(name) - 1;

    return row;
}

void Cfhs_WaveFilterConfig::onCommitButton_clicked()
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

void Cfhs_WaveFilterConfig::onCancelButton_clicked()
{
    this->reject();
}

