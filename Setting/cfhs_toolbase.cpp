#include "cfhs_toolbase.h"
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>


Cfhs_ToolBase::Cfhs_ToolBase(QWidget *parent)
    :QDialog (parent)
{
    //功能名
    m_nameLabel = new QLabel(this);
    m_nameLabel->setStyleSheet(getToolLabelStyle());
    //参数表
    m_algoTable = new Cfhs_AlgorithmTable(this);
    //commit button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_ToolBase::onCommitButton_clicked);
    //cancel button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_ToolBase::onCancelButton_clicked);
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
}

Cfhs_ToolBase::~Cfhs_ToolBase()
{

}

QString Cfhs_ToolBase::getToolLabelStyle()
{
    QString style = QString("QLabel{background:#3954C9;border:none;font-size:16px;"
                            "font-family:Mircosoft Yahei; min-height:26px;"
                            "color:#0077FF;padding-left:15px;}");

    return style;
}

void Cfhs_ToolBase::setFunctionName(const QString &name)
{
    m_nameLabel->setText(name);
}

void Cfhs_ToolBase::setParaVector(const QVector<Cfhs_ToolBase::ParaInfo> &vector)
{
    m_vectorPara = vector;
    m_algoTable->tableInit();
    foreach(ParaInfo info, m_vectorPara)
    {
        //插入算法表
        m_algoTable->addOnePara(info.m_showName, info.m_toolTip);
    }
}

int Cfhs_ToolBase::getParaNum() const
{
    return m_vectorPara.count();
}

void Cfhs_ToolBase::insertOnePara(const QString &toolName, const QString &showName, const QString &toolTip)
{
    ParaInfo info;
    info.m_index = m_vectorPara.count() + 1;
    info.m_toolName = toolName;
    info.m_showName = showName;
    info.m_toolTip = toolTip;
    m_vectorPara.push_back(info);
    m_algoTable->addOnePara(info.m_showName, info.m_toolTip);
}

int Cfhs_ToolBase::getIndexFromName(const QString &name)
{
    int index = -1;
    foreach(ParaInfo info, m_vectorPara)
    {
        if(info.m_toolName == name)
        {
            index = info.m_index;
            break;
        }
    }

    return index;
}

QString Cfhs_ToolBase::getNameFromIndex(const int &index)
{
    QString name = "";
    foreach(ParaInfo info, m_vectorPara)
    {
        if(info.m_index == index)
        {
            name = info.m_toolName;
            break;
        }
    }

    return name;
}

int Cfhs_ToolBase::getRowFromName(const QString &name)
{
    return getIndexFromName(name) -1;
}

void Cfhs_ToolBase::onCommitButton_clicked()
{
    QMap<int, QString> map;
    if(!m_algoTable->getParaMap(map))
        return;
    //转成Json格式
    QString strInfo;
    QJsonObject obj;
    if(map.size() != getParaNum())
    {
        strInfo = QString(tr("设置的参数个数不对"));
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

void Cfhs_ToolBase::onCancelButton_clicked()
{
    this->reject();
}
