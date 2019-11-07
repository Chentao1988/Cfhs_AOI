#include "cfhs_roiconfig.h"
#include "../cfhs_base.h"
#include "../cfhs_global.h"
#include <QRadioButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>


Cfhs_RoiConfig::Cfhs_RoiConfig(QWidget *parent)
    : QDialog (parent)
{
    //自动
    m_autoButton = new QRadioButton(this);
    m_autoButton->setText(tr("自动"));
    m_autoButton->setChecked(true);
    //手动
    m_manualButton = new QRadioButton(this);
    m_manualButton->setText(tr("手动"));
    //确定
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_RoiConfig::on_commitButton_clicked);
    //取消
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_RoiConfig::on_cancelButton_clicked);
    //button layout
    QPointer<QHBoxLayout> bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_cancelButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_commitButton);
    bottomLayout->addStretch();
    //添加全局layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_autoButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_manualButton, 0, Qt::AlignCenter);
    mainLayout->addLayout(bottomLayout);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(10, 10, 10, 30);
    this->setLayout(mainLayout);
    this->setWindowTitle(tr("Roi设置"));
    this->resize(300, 200);
}

Cfhs_RoiConfig::~Cfhs_RoiConfig()
{

}

QString Cfhs_RoiConfig::getParaConfig() const
{
    return m_strConfig;
}

QString Cfhs_RoiConfig::getShowName()
{
    QString name = tr("Roi设置");

    return name;
}

QString Cfhs_RoiConfig::getToolTip()
{
    QString tip = QString(tr("从大图上通过Roi切图"));
    return tip;
}

QString Cfhs_RoiConfig::getIconPath()
{
    QString path = ":/image_tool/roi_config.png";

    return path;
}

QString Cfhs_RoiConfig::getToolName()
{
    return "RoiConfig";
}

bool Cfhs_RoiConfig::setParaConfig(const QString &strConfig)
{
    QMap<QString, QString> map;
    QString strInfo;
    if(!getMapFromJson(strConfig, map, strInfo))
        return false;
    if(map.contains("AutoRoi"))
    {
        int isAuto = map.value("AutoRoi").toInt();
        if(isAuto == 0)
            m_manualButton->setChecked(true);
        else
            m_autoButton->setChecked(true);
    }

    return true;
}

void Cfhs_RoiConfig::on_commitButton_clicked()
{
    bool isAuto = true;
    if(m_autoButton->isChecked())
        isAuto = true;
    else
        isAuto = false;
    QJsonObject obj;
    obj.insert("AutoRoi", QString::number(isAuto));
    QJsonDocument doc;
    doc.setObject(obj);
    m_strConfig = QString(doc.toJson(QJsonDocument::Compact));

    this->accept();
}

void Cfhs_RoiConfig::on_cancelButton_clicked()
{
    this->reject();
}
