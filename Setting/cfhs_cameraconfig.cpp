#include "cfhs_cameraconfig.h"
#include "../cfhs_combobox.h"
#include "../cfhs_global.h"
#include <QTreeWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QIcon>
#include <QSize>


Cfhs_CameraConfig::Cfhs_CameraConfig(QWidget *parent)
    : QDialog (parent)
{
    //相机
    m_cameraTree = new QTreeWidget(this);
    m_cameraTree->setHeaderLabel(tr("相机选择"));
    m_cameraTree->setIconSize(QSize(90,30));
    connect(m_cameraTree, &QTreeWidget::itemClicked,
            this, &Cfhs_CameraConfig::on_cameraTree_itemClicked);
    //CCF配置文件
    m_ccfNameLabel = new QLabel(this);
    m_ccfNameLabel->setText(tr("CCF配置文件"));
    m_ccfCombo = new Cfhs_ComboBox(this);
    QPointer<QHBoxLayout> topLayout = new QHBoxLayout();
    topLayout->addWidget(m_ccfNameLabel);
    topLayout->addWidget(m_ccfCombo, 1);
    //cancel button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_CameraConfig::on_cancelButton_clicked);
    //commit button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_CameraConfig::on_commitButton_clicked);
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addStretch();
    //添加主layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_cameraTree, 1);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(6, 10, 6, 20);
    this->setLayout(mainLayout);
    this->setWindowTitle(tr("相机设置"));
    this->resize(600, 650);
}

Cfhs_CameraConfig::~Cfhs_CameraConfig()
{

}

void Cfhs_CameraConfig::setCameraConfig(const QString &strConfig)
{
    if(strConfig.isEmpty() || strConfig == "null")
        return;

    QStringList list = strConfig.split("@");
    QString strName;  //相机品牌
    QString strType;  //相机类型
    QString strCcf;  //CCF文件
    foreach(QString info, list)
    {
        QStringList singleList = info.split("#");
        strName = singleList.at(0);
        strType = singleList.at(1);
        strCcf = singleList.at(2);
        //插入到map中
        QMap<QString, QStringList> mapType;
        mapType.clear();
        QStringList listCcf;
        listCcf.clear();
        if(m_mapCamera.contains(strName))
        {
            //相机类型是否已存在记录
            if(m_mapCamera.value(strName).contains(strType))
                m_mapCamera[strName][strType].append(strCcf);
            else
            {
                //插入新的记录
                listCcf.append(strCcf);
                m_mapCamera[strName].insert(strType, listCcf);
            }
        }
        else
        {
            //插入新的记录
            listCcf.append(strCcf);
            mapType.insert(strType, listCcf);
            m_mapCamera.insert(strName, mapType);
        }
    }
    //插入到tree上
    m_cameraTree->clear();
    QMap<QString, QMap<QString, QStringList>>::const_iterator iter = m_mapCamera.begin();
    while(iter != m_mapCamera.end())
    {
        //相机品牌
        strName = iter.key();
        QTreeWidgetItem *rootItem = getItem(strName, true);
        //相机类型
        QMap<QString, QStringList> mapType = iter.value();
        QMap<QString, QStringList>::const_iterator typeIter = mapType.begin();
        while(typeIter != mapType.end())
        {
            strType = typeIter.key();
            QTreeWidgetItem *item = getItem(strType);
            //添加到root上
            rootItem->addChild(item);
            typeIter++;
        }
        //添加到tree上
        m_cameraTree->addTopLevelItem(rootItem);
        iter++;
    }
}

QString Cfhs_CameraConfig::getParaConfig() const
{
    return m_strConfig;
}

bool Cfhs_CameraConfig::setParaConfig(const QString &strPara)
{
    m_strConfig = strPara;
    QMap<QString, QString> map;
    QString strInfo;
    if(!getMapFromJson(m_strConfig, map, strInfo))
        return false;
    m_strCameraBrand = map.value("CameraBrand");
    m_strCameraType = map.value("CameraType");
    m_strCcfFile = map.value("CcfConfig");
    //设置当前相机类型
    QList<QTreeWidgetItem*> listItem = m_cameraTree->findItems(m_strCameraBrand, Qt::MatchContains, 0);
    QTreeWidgetItem *rootItem = listItem.at(0);
    if(rootItem)
    {
        int count = rootItem->childCount();
        for(int i=0; i<count; i++)
        {
            QTreeWidgetItem *currentItem = rootItem->child(i);
            if(currentItem->text(0) == m_strCameraType)
            {
                m_cameraTree->setCurrentItem(currentItem);
                break;
            }
        }
        //设置当前CCF文件
        m_ccfCombo->clear();
        QStringList listCcf = m_mapCamera.value(m_strCameraBrand).value(m_strCameraType);
        m_ccfCombo->addItems(listCcf);
        m_ccfCombo->setCurrentText(m_strCcfFile);
        return true;
    }
    else
        return true;
}

QString Cfhs_CameraConfig::getShowName()
{
    QString name = tr("相机设置");

    return name;
}

QString Cfhs_CameraConfig::getToolTip()
{
    QString tip = QString(tr("选择相机型号和对应的CCF配置文件"));
    return tip;
}

QString Cfhs_CameraConfig::getIconPath()
{
    QString path = ":/image_tool/camera_config.png";

    return path;
}

QString Cfhs_CameraConfig::getToolPosition()
{
    return "1-1";
}

QTreeWidgetItem *Cfhs_CameraConfig::getItem(const QString &name,
                                            const bool &hasIcon,
                                            const int &column)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(column, name);
    if(hasIcon)
    {
        QString path = QString(":/image_tool/camera_%1.png").arg(name.toLower());
        QIcon icon;
        icon.addFile(path);
        item->setIcon(0, icon);
    }

    return item;
}

void Cfhs_CameraConfig::on_cameraTree_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->childCount() > 0)
    {
        //点击的相机品牌
        m_strCameraType.clear();
        m_ccfCombo->clear();
        return;
    }
    QString strType = item->text(column);
    //获取相机品牌
    QTreeWidgetItem *rootItem = item->parent();
    QString strName = rootItem->text(column);
    QStringList list = m_mapCamera.value(strName).value(strType);
    //加载CCF文件
    m_ccfCombo->clear();
    m_ccfCombo->addItems(list);
    //保存相机品牌
    m_strCameraBrand = strName;
    //保存选择的相机型号
    m_strCameraType = strType;
}

void Cfhs_CameraConfig::on_commitButton_clicked()
{
    if(m_strCameraType.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请选择相机类型"));
        return;
    }
    m_strCcfFile = m_ccfCombo->currentText();
    if(m_strCcfFile.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请选择CCF配置文件"));
        return;
    }
    QJsonObject obj;
    obj.insert("CameraBrand", m_strCameraBrand);
    obj.insert("CameraType", m_strCameraType);
    obj.insert("CcfConfig", m_strCcfFile);
    QJsonDocument doc;
    doc.setObject(obj);
    m_strConfig = QString(doc.toJson(QJsonDocument::Compact));

    this->accept();
}

void Cfhs_CameraConfig::on_cancelButton_clicked()
{
    this->reject();
}
