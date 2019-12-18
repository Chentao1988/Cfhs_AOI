#include "cfhs_stationprocess.h"
#include "../cfhs_messageinput.h"
#include "../cfhs_base.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include <QMimeData>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QApplication>
#include <QDrag>

const QString StrCameraTool = "1-";  //相机工具
const QString StrRegionTool = "2-";  //分区工具
const QString StrAlgorithmTool = "3-";  //算法工具

Cfhs_StationProcess::Cfhs_StationProcess(const stStation &station, QWidget *parent)
    :QListWidget (parent)
{
    //menu init
    m_menu = new QMenu(this);
    //修改名字
    m_alterNameAction = new QAction(this);
    m_alterNameAction->setText(tr("修改工具名"));
    connect(m_alterNameAction, &QAction::triggered,
            this, &Cfhs_StationProcess::slot_alterNameAction_triggered);
    //修改工具介绍
    m_alterTipAction = new QAction(this);
    m_alterTipAction->setText(tr("修改工具介绍"));
    connect(m_alterTipAction, &QAction::triggered,
            this, &Cfhs_StationProcess::slot_alterTipAction_triggered);
    //删除工具
    m_deleteAction = new QAction(this);
    m_deleteAction->setText(tr("删除"));
    connect(m_deleteAction, &QAction::triggered,
            this, &Cfhs_StationProcess::slot_deleteAction_triggered);
    //清空工具
    m_clearAction = new QAction(this);
    m_clearAction->setText(tr("清空"));
    connect(m_clearAction, &QAction::triggered,
            this, &Cfhs_StationProcess::slot_clearAction_triggered);
    //添加到menu上
    m_menu->addAction(m_alterNameAction);
    m_menu->addAction(m_alterTipAction);
    m_menu->addAction(m_deleteAction);
    m_menu->addAction(m_clearAction);
    //设置样式
    QString style = QString("QListWidget{background:transparent; border:none;"
                            "padding-left:8px;font-size:16px; font-family:Mircosoft Yahei;}"
                            "QListWidget:item{color:#2193AB;min-height: 36px;"
                            "border-left:1px solid #0077FF; border-bottom:1px solid #0077FF}");
    setStyleSheet(style);
    this->setIconSize(QSize(26, 26));
    this->setAcceptDrops(true);
    this->setDragEnabled(true);
    //value init
    m_isPressed = false;
    m_actionItem = nullptr;
    m_startPos = QPoint(0, 0);
    //设置工位信息
    setStation(station);
}

Cfhs_StationProcess::~Cfhs_StationProcess()
{

}

void Cfhs_StationProcess::setStation(const stStation &station)
{
    m_stationNo = station.iStationNo;
    //清除原有数据
    clearContent();
    QString strPara = station.strToolPara;
    if(strPara.isEmpty() || strPara == "null")
        return;
    //解析数据
    QString strInfo;
    QJsonParseError error;
    QByteArray json;
    json.append(strPara);
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if(error.error != QJsonParseError::NoError)
    {
        strInfo = QString(tr("工位%1的流程数据解析有误：%2")).arg(station.iStationNo)
                .arg(error.errorString());
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    if(!doc.isObject())
    {
        strInfo = QString(tr("工位%1的流程数据解析有误：%2")).arg(station.iStationNo)
                .arg("data is not a object");
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QJsonObject obj = doc.object();
    if(obj.contains("ToolArray"))
    {
        QJsonArray array = obj.take("ToolArray").toArray();
        for(int i=1; i<=array.size(); i++)
        {
            StationTool tool;
            //按顺序添加tool
            for(int j=0; j<array.size(); j++)
            {
                QJsonObject dataObj = array.at(j).toObject();
                int index = dataObj.take("ToolIndex").toString().toInt();
                if(index == i)
                {
                    //工具索引
                    tool.m_index = index;
                    //工具位置
                    tool.m_toolPosition = dataObj.take("ToolPosition").toString();
                    //工具参数
                    QJsonValue value = dataObj.take("ToolPara");
                    if(value.isString())
                    {
                        tool.m_toolPara = value.toString();
                    }
                    else if(value.isObject())
                    {
                        QJsonObject toolObj = value.toObject();
                        QJsonDocument toolDoc;
                        toolDoc.setObject(toolObj);
                        tool.m_toolPara = QString(toolDoc.toJson(QJsonDocument::Compact));
                    }
                    else
                    {
                        tool.m_toolPara = "null";
                    }
                    break;
                }
            }
            //添加一个工具
            //检查工具索引是否合法
            if(tool.m_index >=1 && tool.m_index<=array.size())
                addStationTool(tool);
        }
    }
}

void Cfhs_StationProcess::addStationTool(StationTool &tool)
{
    //查询该工具已添加的次数
    int times = 0;
    foreach(Cfhs_StationSingleTool* single, m_listTool)
    {
        if(single->getStationTool().m_toolPosition == tool.m_toolPosition)
            times++;
    }
    tool.m_times = times;
    Cfhs_StationSingleTool *station = new Cfhs_StationSingleTool(tool, this);
    this->addItem(station);
    m_listTool.append(station);
}

QString Cfhs_StationProcess::getStationProcess() const
{

    if(m_listTool.isEmpty())
        return "null";
    QString strProcess;
    QJsonArray array;
    QJsonParseError error;
    foreach(Cfhs_StationSingleTool *tool, m_listTool)
    {
        int index = tool->getStationTool().m_index;
        QString position = tool->getStationTool().m_toolPosition;
        QString para = tool->getStationTool().m_toolPara;
        //添加到obj中
        QJsonObject toolObj;
        toolObj.insert("ToolIndex", QString::number(index));
        toolObj.insert("ToolPosition", position);
        //添加参数,由QString转成object
        if(para.isEmpty() || para == "null")
            toolObj.insert("ToolPara", "null");
        else
        {
            QByteArray json;
            json.append(para);
            QJsonDocument paraDoc = QJsonDocument::fromJson(json, &error);
            if(error.error != QJsonParseError::NoError)
            {
                //数据有误，重置为空
                toolObj.insert("ToolPara", "null");
            }
            else
            {
                toolObj.insert("ToolPara", paraDoc.object());
            }
        }
        //添加到array中
        array.append(toolObj);
    }
    QJsonObject obj;
    obj.insert("ToolArray", array);
    QJsonDocument doc;
    doc.setObject(obj);
    strProcess = QString(doc.toJson(QJsonDocument::Compact));

    return strProcess;
}

int Cfhs_StationProcess::getStationNo() const
{
    return m_stationNo;
}

QString Cfhs_StationProcess::getStationFeature() const
{
    if(m_listTool.isEmpty())
        return "null";
    QStringList listEn;  //英语版特征列表
    QStringList listCns;  //简体中文版
    QStringList listCnt;  //繁体中文版
    foreach(Cfhs_StationSingleTool *tool, m_listTool)
    {
        QString toolPosition = tool->getStationTool().m_toolPosition;
        if(toolPosition.contains(StrAlgorithmTool))
        {
            QStringList listEnOne;
            QStringList listCnsOne;
            QStringList listCntOne;
            if(getAlgorithmToolOutput(toolPosition, listEnOne, listCnsOne, listCntOne))
            {
                for(int i=0; i<listEnOne.count(); i++)
                {
                    //已存在略过
                    if(listEn.contains(listEnOne.at(i)))
                        continue;
                    listEn.append(listEnOne.at(i));
                    listCns.append(listCnsOne.at(i));
                    listCnt.append(listCntOne.at(i));
                }
            }
        }
    }
    //如果数据为空，返回null
    if(listEn.isEmpty())
        return "null";
    QString strFeatureEn, strFeatureCns, strFeatureCnt;
    for(int i=0; i<listEn.count(); i++)
    {
        if(i ==0)
        {
            strFeatureEn.append(listEn.at(i));
            strFeatureCns.append(listCns.at(i));
            strFeatureCnt.append(listCnt.at(i));
        }
        else
        {
            strFeatureEn.append("#");
            strFeatureEn.append(listEn.at(i));
            strFeatureCns.append("#");
            strFeatureCns.append(listCns.at(i));
            strFeatureCnt.append("#");
            strFeatureCnt.append(listCnt.at(i));
        }
    }
    //添加个数和距离
    //英文版
    strFeatureEn.append("#");
    strFeatureEn.append("DefectNum");
    strFeatureEn.append("#");
    strFeatureEn.append("DefectDis");
    //简体中文版
    strFeatureCns.append("#");
    strFeatureCns.append("个数");
    strFeatureCns.append("#");
    strFeatureCns.append("距离");
    //繁体中文版
    strFeatureCnt.append("#");
    strFeatureCnt.append("個數");
    strFeatureCnt.append("#");
    strFeatureCnt.append("距離");


    QJsonObject obj;
    obj.insert("English", strFeatureEn);
    obj.insert("Simplified", strFeatureCns);
    obj.insert("Traditional", strFeatureCnt);
    QJsonDocument doc;
    doc.setObject(obj);

    QString strFeather = QString(doc.toJson(QJsonDocument::Compact));
    return strFeather;
}

bool Cfhs_StationProcess::getAlgorithmToolOutput(const QString &toolPosition, QStringList &listEn,
                                                 QStringList &listCns, QStringList &listCnt)
{
    //清空源数据
    listEn.clear();
    listCns.clear();
    listCnt.clear();
    if(toolPosition == Cfhs_DefectConfig::getToolPosition())
    {
        listEn = Cfhs_DefectConfig::getToolOutput(English);
        listCns = Cfhs_DefectConfig::getToolOutput(SimplifiedChinese);
        listCnt = Cfhs_DefectConfig::getToolOutput(TraditionalChinese);
    }
    else if(toolPosition == Cfhs_DefectConfig_hjh::getToolPosition())
    {
        listEn = Cfhs_DefectConfig_hjh::getToolOutput(English);
        listCns = Cfhs_DefectConfig_hjh::getToolOutput(SimplifiedChinese);
        listCnt = Cfhs_DefectConfig_hjh::getToolOutput(TraditionalChinese);
    }
    else if(toolPosition == Cfhs_ItoDetectConfig::getToolPosition())
    {
        listEn = Cfhs_ItoDetectConfig::getToolOutput(English);
        listCns = Cfhs_ItoDetectConfig::getToolOutput(SimplifiedChinese);
        listCnt = Cfhs_ItoDetectConfig::getToolOutput(TraditionalChinese);
    }

    if(listEn.isEmpty())
        return false;
    else
        return true;
}

void Cfhs_StationProcess::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat("QTreeWidgetItem"))
    {
        //添加工具
        QByteArray info = event->mimeData()->data("QTreeWidgetItem");
        if(info.isEmpty())
        {
            event->accept();
            return;
        }
        QString toolPosition;
        toolPosition.append(info);
        if(!isAcceptedAddTool(toolPosition))
        {
            event->ignore();
            return;
        }
        int size = m_listTool.size();
        StationTool tool;
        tool.m_index = size + 1;
        tool.m_toolPosition = toolPosition;
        //添加一个工具
        addStationTool(tool);
        event->accept();
    }
    else if(event->mimeData()->hasFormat("QListWidgetItem"))
    {
        //修改工具顺序
        //拖拽item的行数
        int oldRow = this->row(m_actionItem);
        //释放位置的行数
        Cfhs_StationSingleTool *item = static_cast<Cfhs_StationSingleTool*>(this->itemAt(event->pos()));
        if(!item)
            return;
        int curRow = this->row(item);
        if(oldRow == curRow)
            return;
        //拖拽的是相机配置
        if(m_actionItem->getStationTool().m_toolPosition.contains(StrCameraTool)
                && oldRow == 0)
        {
            event->ignore();
            return;
        }
        //拖拽的是分区工具
        else if(m_actionItem->getStationTool().m_toolPosition.contains(StrRegionTool)
                && oldRow == 1)
        {
            event->ignore();
            return;
        }
        //放下的位置是相机配置
        if(item->getStationTool().m_toolPosition.contains(StrCameraTool)
                && curRow == 0)
        {
            event->ignore();
            return;
        }
        //放下的位置是分区工具
        else if(item->getStationTool().m_toolPosition.contains(StrRegionTool)
                && curRow == 1)
        {
            event->ignore();
            return;
        }
        //删除旧行
        this->takeItem(oldRow);
        //插入到新的位置
        this->insertItem(curRow, m_actionItem);
        //修改所有item的index
        int count = this->count();
        for(int i=0; i<count; i++)
        {
            Cfhs_StationSingleTool *tool = static_cast<Cfhs_StationSingleTool*>(this->item(i));
            tool->setToolIndex(i+1);
        }
        event->accept();
    }
    else
    {
        event->ignore();
        QListWidget::dropEvent(event);
    }
}

void Cfhs_StationProcess::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("QTreeWidgetItem")
            || event->mimeData()->hasFormat("QListWidgetItem"))
    {
        //接收信息
        event->acceptProposedAction();
    }
    else {
        event->ignore();
        QListWidget::dragMoveEvent(event);
    }
}

void Cfhs_StationProcess::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("QTreeWidgetItem")
            || event->mimeData()->hasFormat("QListWidgetItem"))
    {
        //接收信息
        event->acceptProposedAction();
    }

    else
    {
        event->ignore();
        QListWidget::dragEnterEvent(event);
    }
}

void Cfhs_StationProcess::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        m_startPos = event->pos();
        m_actionItem = static_cast<Cfhs_StationSingleTool*>(this->itemAt(m_startPos));
    }
}

void Cfhs_StationProcess::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_isPressed)
        return;
    if(!m_actionItem)
        return;
    if(!(event->buttons()&Qt::LeftButton))
        return;
    if((event->pos()-m_startPos).manhattanLength() < QApplication::startDragDistance())
        return;
    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData();
    mime->setData("QListWidgetItem", QByteArray());
    drag->setMimeData(mime);
    QString toolPosition = m_actionItem->getStationTool().m_toolPosition;
    QString iconPath = Cfhs_StationSingleTool::getIconPath(toolPosition);
    QPixmap map;
    if(map.load(iconPath))
    {
        map = map.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        drag->setPixmap(map);
    }
    drag->exec(Qt::MoveAction);
}

void Cfhs_StationProcess::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    QListWidget::mouseReleaseEvent(event);
}

void Cfhs_StationProcess::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_actionItem = static_cast<Cfhs_StationSingleTool*>(this->itemAt(event->pos()));
    if(!m_actionItem)
        return;
    QString toolPosition = m_actionItem->getStationTool().m_toolPosition;
    QString strPara = m_actionItem->getStationTool().m_toolPara;
    if(toolPosition == Cfhs_CameraConfig::getToolPosition())
    {
        QString strCameraConfig;
        QString strInfo;
        //获取相机信息
        if(!m_logicInterface->GetCameraInfo(strCameraConfig, strInfo))
        {
            QMessageBox::warning(this, " ", strInfo);
            return;
        }
        //相机设置
        Cfhs_CameraConfig camera(this);
        camera.setCameraConfig(strCameraConfig);
        camera.setParaConfig(strPara);
        if(camera.exec() == QDialog::Accepted)
        {
            strPara = camera.getParaConfig();
        }
    }
    else if(toolPosition == Cfhs_AutoRegionConfig::getToolPosition())
    {
        //自动AA区
        Cfhs_AutoRegionConfig region(this);
        if(!region.setParaConfig(strPara))
            return;
        if(region.exec() == QDialog::Accepted)
        {
            strPara = region.getParaConfig();
        }
    }
    else if(toolPosition == Cfhs_DefectConfig::getToolPosition())
    {
        //瑕疵检测
        Cfhs_DefectConfig defect(this);
        if(!defect.setParaConfig(strPara))
            return;
        if(defect.exec() == QDialog::Accepted)
        {
            strPara = defect.getParaConfig();
        }
    }
    else if(toolPosition == Cfhs_WaveFilterConfig::getToolPosition())
    {
        //膨胀滤波
        Cfhs_WaveFilterConfig wave(this);
        if(!wave.setParaConfig(strPara))
            return;
        if(wave.exec() == QDialog::Accepted)
        {
            strPara = wave.getParaConfig();
        }
    }
    else if(toolPosition == Cfhs_AutoRegionConfig_hjh::getToolPosition())
    {
        //自动AA区2
        Cfhs_AutoRegionConfig_hjh region(this);
        if(!region.setParaConfig(strPara))
            return;
        if(region.exec() == QDialog::Accepted)
        {
            strPara = region.getParaConfig();
        }
    }
    else if(toolPosition == Cfhs_DefectConfig_hjh::getToolPosition())
    {
        //瑕疵检测2
        Cfhs_DefectConfig_hjh defect(this);
        if(!defect.setParaConfig(strPara))
            return;
        if(defect.exec() == QDialog::Accepted)
        {
            strPara = defect.getParaConfig();
        }
    }
    else if(toolPosition == Cfhs_ItoDetectConfig::getToolPosition())
    {
        //ITO检测
        Cfhs_ItoDetectConfig ito(this);
        if(!ito.setParaConfig(strPara))
            return;
        if(ito.exec() == QDialog::Accepted)
            strPara = ito.getParaConfig();
    }
    m_actionItem->setToolPara(strPara);
}

void Cfhs_StationProcess::contextMenuEvent(QContextMenuEvent *event)
{
    m_actionItem = static_cast<Cfhs_StationSingleTool*>(this->itemAt(event->pos()));
    if(m_actionItem)
    {
        QCursor cursor;
        m_menu->exec(cursor.pos());
    }
}

bool Cfhs_StationProcess::isAcceptedAddTool(const QString &toolPosition)
{
    int count = m_listTool.count();
    if(count==0 && !toolPosition.contains(StrCameraTool))
    {
        QMessageBox::warning(this, " ", tr("请添加相机设置"));
        return false;
    }
    else if(count == 1 && !toolPosition.contains(StrRegionTool))
    {
        QMessageBox::warning(this, " ", tr("请添加分区工具"));
        return false;
    }
    bool isAccepted = true;
    //如果是相机设置和分区工具，则检查是否已经存在
    if(toolPosition.contains(StrCameraTool))
    {
        foreach(Cfhs_StationSingleTool* tool, m_listTool)
        {
            QString strTool = tool->getStationTool().m_toolPosition;
            if(strTool.contains(StrCameraTool))
            {
                isAccepted = false;
                QMessageBox::warning(this, " ", tr("相机设置已存在"));
                break;
            }
        }
    }
    else if(toolPosition.contains(StrRegionTool))
    {
        foreach(Cfhs_StationSingleTool* tool, m_listTool)
        {
            QString strTool = tool->getStationTool().m_toolPosition;
            if(strTool.contains(StrRegionTool))
            {
                isAccepted = false;
                QMessageBox::warning(this, " ", tr("分区工具已存在"));
                break;
            }
        }
    }
    return isAccepted;
}

void Cfhs_StationProcess::clearContent()
{
    int count = m_listTool.count();
    for(int i= count-1; i>=0; i--)
    {
        Cfhs_StationSingleTool *tool = m_listTool.at(i);
        delete tool;
        tool = nullptr;
    }
    m_listTool.clear();
    this->clear();
}

void Cfhs_StationProcess::slot_alterNameAction_triggered()
{
    if(!m_actionItem)
        return;
    //修改工具名
    Cfhs_MessageInput msg(tr("修改工具名"), this);
    if(msg.exec() == QDialog::Accepted)
    {
        QString strMessage = msg.getMessage();
        m_actionItem->setShowName(strMessage);
    }
}

void Cfhs_StationProcess::slot_alterTipAction_triggered()
{
    if(!m_actionItem)
        return;
    //修改工具介绍
    Cfhs_MessageInput msg(tr("修改工具介绍"), this);
    if(msg.exec() == QDialog::Accepted)
    {
        QString strMessage = msg.getMessage();
        m_actionItem->setToolTip(strMessage);
    }
}

void Cfhs_StationProcess::slot_deleteAction_triggered()
{
    if(!m_actionItem)
        return;
    //该工具后的所有工具索引-1
    foreach(Cfhs_StationSingleTool* station, m_listTool)
    {
        int index = station->getStationTool().m_index;
        if(index > m_actionItem->getStationTool().m_index)
        {
            index--;
            station->setToolIndex(index);
        }
    }
    //将该工具从主件上移除
    int row = this->row(m_actionItem);
    this->takeItem(row);
    m_listTool.removeOne(m_actionItem);
    //删除item
    delete m_actionItem;
    m_actionItem = nullptr;
}

void Cfhs_StationProcess::slot_clearAction_triggered()
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                       tr("提示"),
                                       tr("是否清除所有工具？"),
                                       QMessageBox::Yes|
                                       QMessageBox::No,
                                       this);
    msg->setButtonText(QMessageBox::Yes, tr("是"));
    msg->setButtonText(QMessageBox::No, tr("否"));
    msg->setDefaultButton(QMessageBox::Yes);
    if(msg->exec() == QMessageBox::Yes)
    {
        clearContent();
    }
    delete msg;
    msg = nullptr;
}

void Cfhs_StationProcess::slot_getRoiPoint(const int &stationNo, QMap<int, QList<QList<QPoint> > > &map)
{
    if(stationNo != m_stationNo)
        return;
    foreach(Cfhs_StationSingleTool *tool, m_listTool)
    {
        if(tool->getStationTool().m_toolPosition == Cfhs_RoiConfig::getToolPosition())
        {
            QJsonObject mainObj;
            mainObj.insert("AutoRoi", QString::number(0));
            QMap<int, QList<QList<QPoint>>>::const_iterator mainIter = map.begin();
            QJsonObject subObj;
            while(mainIter != map.end())
            {
                QList<QList<QPoint>> mainList = mainIter.value();
                QString strSubKey = QString::number(mainIter.key()+1);
                QJsonArray subArray;
                foreach(QList<QPoint> listPoint, mainList)
                {
                    QJsonObject sigObj;
                    for(int i=0; i<listPoint.size(); i++)
                    {
                        QPoint point = listPoint.at(i);
                        QString strPoint = QString("%1,%2").arg(point.x()).arg(point.y());
                        QString strKey = QString::number(i+1);
                        sigObj.insert(strKey, strPoint);
                    }
                    subArray.append(sigObj);
                }
                subObj.insert(strSubKey, subArray);
                mainIter++;
            }
            mainObj.insert("Point", subObj);
            QJsonDocument doc;
            doc.setObject(mainObj);
            QString strPara = QString(doc.toJson(QJsonDocument::Compact));
            tool->setToolPara(strPara);
        }
    }
}
