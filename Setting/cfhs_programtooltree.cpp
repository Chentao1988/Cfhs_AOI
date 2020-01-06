#include "cfhs_programtooltree.h"
#include "../cfhs_base.h"
#include "cfhs_stationsingletool.h"
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QPoint>
#include <QIcon>
#include <QApplication>
#include <QDrag>
#include <QMimeData>


Cfhs_ProgramToolTree::Cfhs_ProgramToolTree(QWidget *parent)
    : QTreeWidget (parent)
{
    this->init();
}

Cfhs_ProgramToolTree::~Cfhs_ProgramToolTree()
{

}

void Cfhs_ProgramToolTree::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        m_startPos = event->pos();
    }
    QTreeWidget::mousePressEvent(event);
}

void Cfhs_ProgramToolTree::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_isPressed)
        return;
    if(!(event->buttons() & Qt::LeftButton))
        return;
    QTreeWidgetItem *dragItem = this->itemAt(m_startPos);
    if(!dragItem)
        return;
    //父节点退出
    if(dragItem->childCount() > 0)
        return;
    //小于拖拽距离退出
    if((event->pos()-m_startPos).manhattanLength() < QApplication::startDragDistance())
        return;
    //显示拖拽item
    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData();
    QString toolPosition = m_mapTool.key(dragItem);
    QByteArray info;
    info.append(toolPosition);
    mime->setData("QTreeWidgetItem", info);
    drag->setMimeData(mime);
    QString iconPath = Cfhs_StationSingleTool::getIconPath(toolPosition);
    QPixmap map;
    if(map.load(iconPath))
    {
        map = map.scaled(30, 30);
        drag->setPixmap(map);
    }
    drag->exec(Qt::MoveAction);
}

void Cfhs_ProgramToolTree::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;

    QTreeWidget::mouseReleaseEvent(event);
}

QTreeWidgetItem *Cfhs_ProgramToolTree::getItem(const QString &showName, const QString &iconPath,
                                        const QString &tip, const QString &toolPosition,
                                        const int &column)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(column, showName);
    if(!tip.isEmpty())
        item->setToolTip(column, tip);
    if(!iconPath.isEmpty())
    {
        QIcon icon;
        icon.addFile(iconPath);
        item->setIcon(column, icon);
    }
    //添加到map中
    if(!toolPosition.isEmpty())
        m_mapTool.insert(toolPosition, item);

    return item;
}

void Cfhs_ProgramToolTree::init()
{
    this->setHeaderLabel(tr("工具箱"));  //窗口名
    this->setIconSize(QSize(26, 26));
    //不接受拖拽
    this->setAcceptDrops(false);
    this->setDragEnabled(false);
    //相机模块
    addCameraModule();
    //图像分区模块
    addImagePartitionModule();
    //算法模块
    addAlgorithmModule();
    //value init
    m_isPressed = false;
    m_startPos = QPoint(0, 0);
}

QString Cfhs_ProgramToolTree::getRootIconPath()
{
    QString path = ":/image_tool/tree_root.png";

    return path;
}

void Cfhs_ProgramToolTree::addCameraModule()
{
    QString showName = tr("相机设备");
    QString iconPath = getRootIconPath();
    QString tip = "";
    QString toolPosition ="";
    /***相机设备***/
    QTreeWidgetItem *cameraRootItem = getItem(showName, iconPath);
    //添加工具
    //相机设置
    showName = Cfhs_CameraConfig::getShowName();
    iconPath = Cfhs_CameraConfig::getIconPath();
    tip = Cfhs_CameraConfig::getToolTip();
    toolPosition = Cfhs_CameraConfig::getToolPosition();
    QTreeWidgetItem *cameraConfigItem = getItem(showName, iconPath, tip, toolPosition);
    cameraRootItem->addChild(cameraConfigItem);

    //添加到tree上
    this->addTopLevelItem(cameraRootItem);
}

void Cfhs_ProgramToolTree::addImagePartitionModule()
{
    QString showName = "";
    QString iconPath = "";
    QString tip = "";
    QString toolPosition = "";
    /***图像分区***/
    showName = tr("图像分区");
    iconPath = getRootIconPath();
    QTreeWidgetItem *partitionRootItem = getItem(showName, iconPath);
    //自动AA区
    showName = Cfhs_AutoRegionConfig::getShowName();
    iconPath = Cfhs_AutoRegionConfig::getIconPath();
    tip = Cfhs_AutoRegionConfig::getToolTip();
    toolPosition = Cfhs_AutoRegionConfig::getToolPosition();
    QTreeWidgetItem *autoRegionItem = getItem(showName, iconPath, tip, toolPosition);
    partitionRootItem->addChild(autoRegionItem);
    //自动分区
    showName = Cfhs_AutoRegionConfig_hjh::getShowName();
    iconPath = Cfhs_AutoRegionConfig_hjh::getIconPath();
    tip = Cfhs_AutoRegionConfig_hjh::getToolTip();
    toolPosition = Cfhs_AutoRegionConfig_hjh::getToolPosition();
    QTreeWidgetItem *autoRegionItem2 = getItem(showName, iconPath, tip, toolPosition);
    partitionRootItem->addChild(autoRegionItem2);
    //原图分区
    showName = Cfhs_OriginalRegionConfig::getShowName();
    iconPath = Cfhs_OriginalRegionConfig::getIconPath();
    tip = Cfhs_OriginalRegionConfig::getToolTip();
    toolPosition = Cfhs_OriginalRegionConfig::getToolPosition();
    QTreeWidgetItem *originalRegionItem = getItem(showName, iconPath, tip, toolPosition);
    partitionRootItem->addChild(originalRegionItem);
    //手动Roi
    showName = Cfhs_RoiConfig::getShowName();
    iconPath = Cfhs_RoiConfig::getIconPath();
    tip = Cfhs_RoiConfig::getToolTip();
    toolPosition = Cfhs_RoiConfig::getToolPosition();
    QTreeWidgetItem *roiItem = getItem(showName, iconPath, tip, toolPosition);
    partitionRootItem->addChild(roiItem);

    //添加到tree上
    this->addTopLevelItem(partitionRootItem);
}

void Cfhs_ProgramToolTree::addAlgorithmModule()
{
    QString showName = "";
    QString iconPath = "";
    QString tip = "";
    QString toolPosition = "";
    /***算法模块***/
    showName = tr("算法");
    iconPath = getRootIconPath();
    QTreeWidgetItem *algorithmRootItem = getItem(showName, iconPath);
    //瑕疵检测
    showName = Cfhs_DefectConfig::getShowName();
    iconPath = Cfhs_DefectConfig::getIconPath();
    tip = Cfhs_DefectConfig::getToolTip();
    toolPosition = Cfhs_DefectConfig::getToolPosition();
    QTreeWidgetItem *defectItem = getItem(showName, iconPath, tip, toolPosition);
    algorithmRootItem->addChild(defectItem);
    //边缘检测
    showName = Cfhs_DefectConfig_hjh::getShowName();
    iconPath = Cfhs_DefectConfig_hjh::getIconPath();
    tip = Cfhs_DefectConfig_hjh::getToolTip();
    toolPosition = Cfhs_DefectConfig_hjh::getToolPosition();
    QTreeWidgetItem *defectItem2 = getItem(showName, iconPath, tip, toolPosition);
    algorithmRootItem->addChild(defectItem2);
    //ITO检测
    showName = Cfhs_ItoDetectConfig::getShowName();
    iconPath = Cfhs_ItoDetectConfig::getIconPath();
    tip = Cfhs_ItoDetectConfig::getToolTip();
    toolPosition = Cfhs_ItoDetectConfig::getToolPosition();
    QTreeWidgetItem *itoItem = getItem(showName, iconPath, tip, toolPosition);
    algorithmRootItem->addChild(itoItem);
   ////膨胀滤波
   //showName = Cfhs_WaveFilterConfig::getShowName();
   //iconPath = Cfhs_WaveFilterConfig::getIconPath();
   //tip = Cfhs_WaveFilterConfig::getToolTip();
   //toolPosition = Cfhs_WaveFilterConfig::getToolPosition();
   //QTreeWidgetItem *waveFilterItem = getItem(showName, iconPath, tip, toolPosition);
   //algorithmRootItem->addChild(waveFilterItem);
    //添加到tree上
    this->addTopLevelItem(algorithmRootItem);
}

