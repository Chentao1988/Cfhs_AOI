#include "cfhs_defectcallout.h"
#include "../cfhs_frame.h"
#include "../cfhs_base.h"
#include "../cfhs_messageinput.h"
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QHeaderView>


const QString ConfigFile = "defect.conf";

Cfhs_DefectCallout::Cfhs_DefectCallout(const QString &dir, QWidget *parent)
    : QDialog(parent)
{
    init();
    setDirectory(dir);
}

Cfhs_DefectCallout::~Cfhs_DefectCallout()
{

}

void Cfhs_DefectCallout::setDirectory(const QString &dir)
{
    m_directory = dir;
    QDir sourceDir;
    sourceDir.setPath(dir);
    QFileInfoList listFile = sourceDir.entryInfoList(QDir::Files, QDir::Name);
    QStringList listName;
    foreach(QFileInfo fileInfo, listFile)
    {
        if(fileInfo.isDir())
            continue;
        else
        {
            QString strName = fileInfo.fileName();
            if(strName == ConfigFile)
            {
                QString strPath = fileInfo.absoluteFilePath();
                readConfigFile(strPath);
                continue;
            }
            else
            {
                QString strLower = strName.toLower();
                if(strLower.contains(".png")
                        || strLower.contains(".bmp")
                        || strLower.contains(".jpg")
                        || strLower.contains(".jpeg"))
                {
                    listName.append(strName);
                    continue;
                }
            }
        }
    }
    int count = listName.count();
    m_imageTable->setRowCount(count);
    for(int i=0; i<count; i++)
    {
        QTableWidgetItem *fileItem = new QTableWidgetItem;
        fileItem->setText(listName.at(i));
        m_imageTable->setItem(i, 0, fileItem);
        if(m_mapDefect.contains(i))
        {
            QString strDefect = m_mapDefect.value(i);
            QTableWidgetItem *defectItem = new QTableWidgetItem;
            defectItem->setText(strDefect);
            m_imageTable->setItem(i, 1, defectItem);
        }
    }
}

void Cfhs_DefectCallout::closeEvent(QCloseEvent *event)
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                       tr("提示"),
                                       tr("是否保存缺陷打标数据？"),
                                       QMessageBox::Yes|
                                       QMessageBox::No,
                                       this);
    msg->setButtonText(QMessageBox::Yes, tr("是"));
    msg->setButtonText(QMessageBox::No, tr("否"));
    msg->setDefaultButton(QMessageBox::Yes);
    if(msg->exec() == QMessageBox::Yes)
    {
        if(saveData())
            this->accept();
        else
            event->ignore();
    }
    else
        this->reject();
}

void Cfhs_DefectCallout::init()
{
    //上一张
    m_prevButton = new QPushButton(this);
    m_prevButton->setText(tr("上一张"));
    connect(m_prevButton, &QPushButton::clicked,
            this, &Cfhs_DefectCallout::on_prevButton_clicked);
    //下一张
    m_nextButton = new QPushButton(this);
    m_nextButton->setText(tr("下一张"));
    connect(m_nextButton, &QPushButton::clicked,
            this, &Cfhs_DefectCallout::on_nextButton_clicked);
    //确定
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_DefectCallout::on_commitButton_clicked);
    //取消
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_DefectCallout::on_cancelButton_clicked);
    QPointer<QHBoxLayout> top_layout = new QHBoxLayout;
    top_layout->addWidget(m_prevButton, 0, Qt::AlignCenter);
    top_layout->addWidget(m_nextButton, 0, Qt::AlignCenter);
    top_layout->addWidget(m_commitButton, 0, Qt::AlignCenter);
    top_layout->addWidget(m_cancelButton, 0, Qt::AlignCenter);
    top_layout->setSpacing(20);
    //缺陷打标
    m_calloutFrame = new Cfhs_Frame(this);
    m_calloutFrame->setStyleSheet("QFrame{border:1px solid #0077FF;"
                                  "border-radius: 6px;}");
    //图片列表
    m_imageNameLabel = new QLabel(this);
    m_imageNameLabel->setText(tr("图片名称列表"));
    m_imageTable = new QTableWidget(this);
    m_imageTable->setFixedWidth(400);
    m_imageTable->setColumnCount(2);
    m_imageTable->setRowCount(0);
    QStringList headList;
    headList.append(tr("文件名"));
    headList.append(tr("缺陷名"));
    m_imageTable->setHorizontalHeaderLabels(headList);
    //m_imageTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_imageTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_imageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_imageTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(m_imageTable, &QTableWidget::currentCellChanged,
            this, &Cfhs_DefectCallout::on_imageTable_currentCell_changed);
    connect(m_imageTable, &QTableWidget::cellDoubleClicked,
            this, &Cfhs_DefectCallout::on_imageTable_cellDoubleClicked);
    QPointer<QVBoxLayout> right_layout = new QVBoxLayout;
    right_layout->addWidget(m_imageNameLabel, 0, Qt::AlignCenter);
    right_layout->addWidget(m_imageTable, 1);
    right_layout->setSpacing(10);
    QPointer<QHBoxLayout> bottom_layout = new QHBoxLayout;
    bottom_layout->addWidget(m_calloutFrame, 1);
    bottom_layout->addLayout(right_layout);
    bottom_layout->setSpacing(10);
    //主layout
    QPointer<QVBoxLayout> main_layout = new QVBoxLayout;
    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);
    main_layout->setSpacing(30);
    this->setLayout(main_layout);

    //value init
    m_pointWidget = new Cfhs_SavePoint(this);
    m_calloutFrame->setWidget(m_pointWidget);
    this->setWindowTitle(tr("缺陷打标"));
    this->setFixedSize(1000, 700);
}

bool Cfhs_DefectCallout::saveData()
{
    if(m_mapPoints.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请设置缺陷打标数据"));
        return false;
    }
    QString strConfig = QString("%1/%2").arg(m_directory).arg(ConfigFile);
    return saveConfigFile(strConfig);
}

bool Cfhs_DefectCallout::readConfigFile(const QString &path)
{
    m_mapPoints.clear();
    m_mapDefect.clear();
    QFile file(path);
    if(!file.exists())
        return true;
    if(!file.open(QIODevice::ReadOnly))
    {
        QString strInfo = QString(tr("文件打开失败：%1")).arg(file.fileName());
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    QByteArray strData = file.readAll();
    if(strData.isEmpty())
        return true;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(strData, &error);
    QString strInfo;
    if(error.error != QJsonParseError::NoError)
    {
        strInfo = QString(tr("配置文件数据错误：%1")).arg(error.errorString());
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    if(!doc.isArray())
    {
        strInfo = QString(tr("配置文件数据错误：%1")).arg("data is not a array");
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    QJsonArray array = doc.array();
    for(int i=0; i<array.count(); i++)
    {
        QJsonValue value = array.at(i);
        QJsonObject dataObj = value.toObject();
        //图片索引
        if(!dataObj.contains("Index"))
            continue;
        int index = dataObj.value("Index").toInt();
        //缺陷坐标
        if(!dataObj.contains("ShapeList"))
            continue;
        QJsonArray dataArray = dataObj.value("ShapeList").toArray();
        QList<NPoint> list;
        for(int j=0; j<dataArray.count(); j++)
        {
            NPoint point;
            QJsonObject singleObj = dataArray.at(j).toObject();
            if(!singleObj.contains("Shape"))
                continue;
            point.Shape = singleObj.value("Shape").toInt();
            if(!singleObj.contains("Start"))
                continue;
            double x = singleObj.value("Start").toObject().value("X").toDouble();
            point.Start_Point.setX(x);
            double y= singleObj.value("Start").toObject().value("Y").toDouble();
            point.Start_Point.setY(y);
            if(!singleObj.contains("End"))
                continue;
            x = singleObj.value("End").toObject().value("X").toDouble();
            point.End_Point.setX(x);
            y= singleObj.value("End").toObject().value("Y").toDouble();
            point.End_Point.setY(y);
            list.append(point);
        }
        if(m_mapPoints.contains(index))
            m_mapPoints[index] = list;
        else
            m_mapPoints.insert(index, list);
        //缺陷名称
        if(!dataObj.contains("Name"))
            continue;
        QString strName = dataObj.value("Name").toString();
        if(m_mapDefect.contains(index))
            m_mapDefect[index] = strName;
        else
            m_mapDefect.insert(index, strName);
    }
    file.close();
    return true;
}

bool Cfhs_DefectCallout::saveConfigFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        QString strInfo = QString(tr("文件打开失败：%1")).arg(file.fileName());
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }

    if(m_mapPoints.isEmpty())
    {
        file.close();
        return true;
    }
    QJsonArray array;
    QMap<int, QList<NPoint>>::const_iterator iter;
    for(iter=m_mapPoints.begin(); iter!= m_mapPoints.end(); iter++)
    {
        QJsonObject obj;
        //图片索引
        obj.insert("Index", iter.key());
        QList<NPoint> list = iter.value();
        //缺陷名
        QString strName = "";
        if(m_mapDefect.contains(iter.key()))
            strName = m_mapDefect.value(iter.key());
        obj.insert("Name", strName);
        //缺陷坐标
        QJsonArray dataArray;
        for(int i=0; i<list.count(); i++)
        {
            //插入单一图形
            NPoint point = list.at(i);
            //图形索引
            QJsonObject dataObj;
            dataObj.insert("Shape", point.Shape);
            //起始坐标
            QJsonObject startObj;
            startObj.insert("X", point.Start_Point.x());
            startObj.insert("Y", point.Start_Point.y());
            dataObj.insert("Start", startObj);
            //终点坐标
            QJsonObject endObj;
            endObj.insert("X", point.End_Point.x());
            endObj.insert("Y", point.End_Point.y());
            dataObj.insert("End", endObj);
            //插入到dataArray中
            dataArray.append(dataObj);
        }
        obj.insert("ShapeList", dataArray);
        //插入到array中
        array.append(obj);
    }
    QJsonDocument doc;
    doc.setArray(array);
    QByteArray strInfo = doc.toJson(QJsonDocument::Compact);

    file.write(strInfo);
    file.close();

    return true;
}

void Cfhs_DefectCallout::updateImagePoint(const int &index)
{
    QList<NPoint> list = m_pointWidget->GetList();
    //如果没有缺陷坐标，则清空该记录
    if(list.isEmpty())
    {
        m_mapPoints.remove(index);
        QTableWidgetItem *item = m_imageTable->item(index, 1);
        if(item)
            item->setText("");
    }
    else
    {
        if(m_mapPoints.contains(index))
            m_mapPoints[index] = list;
        else
            m_mapPoints.insert(index, list);
    }
}

void Cfhs_DefectCallout::showImagePoint(const int &index)
{
    QString strName = m_imageTable->item(index, 0)->text();
    QString strPath = QString("%1/%2").arg(m_directory).arg(strName);
    QList<NPoint> list;
    if(m_mapPoints.contains(index))
        list = m_mapPoints.value(index);
    //show data
    m_pointWidget->LoadPath(strPath, list);
}

void Cfhs_DefectCallout::on_prevButton_clicked()
{
    int curRow = m_imageTable->currentRow();
    if(curRow<1)
        return;
    m_prevButton->setEnabled(false);
    QCoreApplication::sendPostedEvents(this);
    curRow--;
    m_imageTable->setCurrentCell(curRow, 0);
    m_prevButton->setEnabled(true);
}

void Cfhs_DefectCallout::on_nextButton_clicked()
{
    int curRow = m_imageTable->currentRow();
    if(curRow >= m_imageTable->rowCount()-1)
        return;
    m_nextButton->setEnabled(false);
    QCoreApplication::sendPostedEvents(this);
    curRow++;
    m_imageTable->setCurrentCell(curRow, 0);
    m_nextButton->setEnabled(true);
}

void Cfhs_DefectCallout::on_commitButton_clicked()
{
    //更新当前画面的信息
    int curRow = m_imageTable->currentRow();
    if(curRow>=0)
        updateImagePoint(curRow);
    //更新缺陷名，只处理有缺陷坐标的图片
    m_mapDefect.clear();
    QMap<int, QList<NPoint>>::const_iterator iter;
    for(iter=m_mapPoints.begin(); iter!=m_mapPoints.end(); iter++)
    {
        int index = iter.key();
        QTableWidgetItem *item = m_imageTable->item(index, 1);
        QString strDefect = "";
        if(item)
            strDefect = item->text();
        //插入到defect map中
        if(m_mapDefect.contains(index))
            m_mapDefect[index] = strDefect;
        else
            m_mapDefect.insert(index, strDefect);
    }
    if(!saveData())
        return;

    this->accept();
}

void Cfhs_DefectCallout::on_cancelButton_clicked()
{
    this->reject();
}

void Cfhs_DefectCallout::on_imageTable_currentCell_changed(int currentRow, int currentColumn,
                                                           int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousColumn);
    if(currentRow == previousRow)
        return;
    //更新数据
    if(previousRow>=0)
    {
        //get point
        updateImagePoint(previousRow);
    }
    //设置新的图片
    if(currentRow>=0)
    {
        showImagePoint(currentRow);
    }
}

void Cfhs_DefectCallout::on_imageTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    if(row < 0)
        return;
    //更新当前画面的信息
    updateImagePoint(row);
    //检测该工具是否有缺陷坐标
    if(m_mapPoints.value(row).isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请标记缺陷位置"));
        return;
    }
    QString strName = tr("缺陷名");
    Cfhs_MessageInput msg(strName, this);
    if(msg.exec() == QDialog::Accepted)
    {
        QString strDefect = msg.getMessage();
        QTableWidgetItem *item = m_imageTable->item(row, 1);
        if(item)
            item->setText(strDefect);
        else
        {
            item = new QTableWidgetItem;
            item->setText(strDefect);
            m_imageTable->setItem(row, 1, item);
        }
    }
}
