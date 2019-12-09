#include "cfhs_stationsvirtualstandardize.h"
#include "cfhs_base.h"
#include "cfhs_combobox.h"
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

Cfhs_StationsVirtualStandardize::Cfhs_StationsVirtualStandardize(QWidget *parent)
    : QDialog(parent)
{
    //窗口初始化
    init();
}

Cfhs_StationsVirtualStandardize::~Cfhs_StationsVirtualStandardize()
{

}

QString Cfhs_StationsVirtualStandardize::getStationStandardizeInfo() const
{
    return m_strStandardize;
}

bool Cfhs_StationsVirtualStandardize::setStationStandardize(const QStringList &stationList,
                                                            const QString &strStandardize)
{
    //清空原有数据
    m_standardizeTable->clear();
    m_standardizeTable->setRowCount(0);
    m_standardizeTable->setColumnCount(0);
    //设置工位列表
    setStationList(stationList);
    //设置标定信息
    if(strStandardize.isEmpty() || strStandardize == "null")
        return true;
    QByteArray json;
    json.append(strStandardize);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    QString strInfo;
    if(error.error != QJsonParseError::NoError)
    {
        strInfo = QString(tr("多工位投影信息解析错误：%1")).arg(error.errorString());
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    if(!doc.isObject())
    {
        strInfo = QString(tr("多工位投影信息解析错误：%1")).arg("data is not a object");
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    QJsonObject mainObj = doc.object();
    //先获取工位索引信息
    if(mainObj.contains("StationIndex"))
    {
        QJsonObject indexObj = mainObj.take("StationIndex").toObject();
        QString strName = indexObj.take("S1").toString();
        m_station1Combo->setCurrentText(strName);
        //获取工位1的缺陷名列表
        int stationNo = strName.toInt();
        QStringList listDefect;
        if(!getStationDefectList(listDefect, stationNo))
            return false;
        setStationDefectList(listDefect, 1);
        strName = indexObj.take("S2").toString();
        m_station2Combo->setCurrentText(strName);
        stationNo = strName.toInt();
        if(!getStationDefectList(listDefect, stationNo))
            return false;
        setStationDefectList(listDefect, 2);
    }
    //获取自定义缺陷信息
    QJsonObject::const_iterator iter = mainObj.begin();
    while(iter != mainObj.end())
    {
        QString newDefect = iter.key();
        QJsonObject dataObj = iter.value().toObject();
        QString strDefect1 = dataObj.value("S1").toString();
        QString strDefect2 = dataObj.value("S2").toString();
        setDefectText(strDefect1, strDefect2, newDefect);

        iter++;
    }

    return true;
}

QString Cfhs_StationsVirtualStandardize::getNgFilterInfo() const
{
    return m_strNg;
}

void Cfhs_StationsVirtualStandardize::init()
{
    //投影工位设置
    //工位1
    m_station1Label = new QLabel(this);
    m_station1Label->setText(tr("投影工位1"));
    m_station1Combo = new Cfhs_ComboBox(this);
    connect(m_station1Combo, &Cfhs_ComboBox::currentTextChanged,
            this, &Cfhs_StationsVirtualStandardize::onStation1Combo_currentText_changed);
    m_station1Name = "";
    //工位2
    m_station2Label = new QLabel(this);
    m_station2Label->setText(tr("投影工位2"));
    m_station2Combo = new Cfhs_ComboBox(this);
    connect(m_station2Combo, &Cfhs_ComboBox::currentTextChanged,
            this, &Cfhs_StationsVirtualStandardize::onStation2Combo_currentText_changed);
    m_station2Name = "";
    //设置工位
    m_stationSetButton = new QPushButton(this);
    m_stationSetButton->setText(tr("开始设定"));
    connect(m_stationSetButton, &QPushButton::clicked,
            this, &Cfhs_StationsVirtualStandardize::onStationSetButton_clicked);
    //添加工位控件layout
    QPointer<QHBoxLayout> stationLayout = new QHBoxLayout();
    stationLayout->addWidget(m_station1Label);
    stationLayout->addWidget(m_station1Combo);
    stationLayout->addWidget(m_station2Label);
    stationLayout->addWidget(m_station2Combo);
    stationLayout->addWidget(m_stationSetButton);
    //标定信息表
    m_standardizeTable = new QTableWidget(this);
    //item默认设为居中显示
    QTableWidgetItem *modelItem = new QTableWidgetItem();
    modelItem->setTextAlignment(Qt::AlignCenter);
    m_standardizeTable->setItemPrototype(modelItem);
    //设置表头不选中
    m_standardizeTable->horizontalHeader()->setHighlightSections(false);
    m_standardizeTable->verticalHeader()->setHighlightSections(false);
    //设置行列
    m_standardizeTable->setRowCount(0);
    m_standardizeTable->setColumnCount(0);
    //确定button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_StationsVirtualStandardize::onCommitButton_clicked);
    //取消button
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_StationsVirtualStandardize::onCancelButton_clicked);
    //添加button控件layout
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->setSpacing(40);
    //添加主窗口layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addLayout(stationLayout);
    mainLayout->addWidget(m_standardizeTable, 1);
    mainLayout->addLayout(buttonLayout);
    this->setLayout(mainLayout);
    //主窗口设置
    this->setWindowTitle(tr("多工位投影标定设置"));
    this->setWindowFlags(this->windowFlags()|Qt::WindowMinMaxButtonsHint);
    this->resize(1280, 720);
}

bool Cfhs_StationsVirtualStandardize::getStationDefectList(QStringList &listDefect, const int &stationNo)
{
    stStation station;
    QString strInfo;
    if(!m_logicInterface->GetStationInfo(m_curProgramName, stationNo, station, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    if(!getKeyListFromJson(station.strDefectDefine, listDefect, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    if(listDefect.isEmpty())
    {
        strInfo = QString(tr("工位%1没有缺陷定义信息")).arg(stationNo);
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }

    return true;
}

//设置投影工位1/2的列表
void Cfhs_StationsVirtualStandardize::setStationList(const QStringList& stationList)
{
    //清空原有数据
    m_station1Combo->clear();
    m_station2Combo->clear();
    //加载新的数据
    m_station1Combo->addItems(stationList);
    m_station1Combo->setCurrentIndex(-1);
    m_station2Combo->addItems(stationList);
    m_station2Combo->setCurrentIndex(-1);
}

//设置投影缺陷列表
//defectList 缺陷列表，默认添加“无”选项
//station  工位索引，取值1，2；1->投影工位1，对应表格纵向，2->投影工位2，对应表格横向
void Cfhs_StationsVirtualStandardize::setStationDefectList(const QStringList& defectList, int station)
{
    //添加“无”选项
    QStringList list = defectList;
    if(!list.contains("null"))
        list.append("null");
    //添加到表格
    int size = list.size();
    //纵向
    if(station == 1)
    {
        //设置行数
        m_standardizeTable->setRowCount(size);
        //设置行标题
        m_standardizeTable->setVerticalHeaderLabels(list);
        //设置行宽
        m_standardizeTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        m_listStation1Title = list;
    }
    //横向
    else
    {
        //设置列数
        m_standardizeTable->setColumnCount(size);
        //设置列标题
        m_standardizeTable->setHorizontalHeaderLabels(list);
        //设置列宽
        m_standardizeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_listStation2Title = list;
    }
}

QJsonObject Cfhs_StationsVirtualStandardize::getObject(const int &row, const int &col)
{
    QJsonObject obj;
    QString strDefect1 = m_listStation1Title.at(row);
    QString strDefect2 = m_listStation2Title.at(col);
    obj.insert("S1", strDefect1);
    obj.insert("S2", strDefect2);

    return obj;
}

bool Cfhs_StationsVirtualStandardize::setDefectText(const QString &strDefect1,
                                                    const QString &strDefect2,
                                                    const QString &newDefect)
{
    int row = m_listStation1Title.indexOf(strDefect1);
    int col = m_listStation2Title.indexOf(strDefect2);
    if(row<0 || col<0)
    {
        return false;
    }
    QTableWidgetItem *item = m_standardizeTable->item(row, col);
    if(!item)
    {
        item = new QTableWidgetItem();
        item->setText(newDefect);
        m_standardizeTable->setItem(row, col, item);
    }
    else
        item->setText(newDefect);

    return true;
}

void Cfhs_StationsVirtualStandardize::onStation1Combo_currentText_changed(const QString& text) //选择投影工位1
{
    m_station1Name = text;
}

void Cfhs_StationsVirtualStandardize::onStation2Combo_currentText_changed(const QString& text) //选择投影工位2
{
    m_station2Name = text;
}

void Cfhs_StationsVirtualStandardize::onStationSetButton_clicked()  //设定工位
{
    if(m_station1Name.isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("请选择投影工位1信息"));
        return;
    }
    if(m_station2Name.isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("请选择投影工位2信息"));
        return;
    }
    if(m_station1Name == m_station2Name)
    {
        QMessageBox::warning(this, tr("错误"), tr("两个投影工位不能相同"));
        return;
    }
    //获取工位缺陷信息
    //投影工位1信息
    int stationNo = m_station1Name.toInt();
    QStringList listDefect;
    if(!getStationDefectList(listDefect, stationNo))
        return;
    setStationDefectList(listDefect, 1);
    //投影工位2信息
    stationNo = m_station2Name.toInt();
    if(!getStationDefectList(listDefect, stationNo))
        return;
    setStationDefectList(listDefect, 2);
    //清空原来数据
    m_standardizeTable->clearContents();
}

void Cfhs_StationsVirtualStandardize::onCommitButton_clicked()  //确定
{
    m_station1Name = m_station1Combo->currentText();
    if(m_station1Name.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请选择投影工位1"));
        return;
    }
    m_station2Name = m_station2Combo->currentText();
    if(m_station2Name.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请选择投影工位2"));
        return;
    }
    if(m_listStation1Title.isEmpty() || m_listStation2Title.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请设置投影工位"));
        return;
    }
    QJsonObject mainObj;
    //添加工位索引
    QJsonObject indexObj;
    indexObj.insert("S1", m_station1Name);
    indexObj.insert("S2", m_station2Name);
    mainObj.insert("StationIndex", indexObj);
    QJsonObject ngObj;
    //添加缺陷自定义信息
    for(int i=0; i<m_standardizeTable->rowCount(); i++)
    {
        for(int j=0; j<m_standardizeTable->columnCount(); j++)
        {
            QTableWidgetItem *item = m_standardizeTable->item(i, j);
            //没有数据时跳过
            if(!item)
                continue;
            if(item->text().isEmpty())
                continue;
            mainObj.insert(item->text(), getObject(i, j));
            //有效数据添加到NG筛选中
            ngObj.insert(item->text(), "");
        }
    }
    QJsonDocument doc;
    doc.setObject(mainObj);
    m_strStandardize = QString(doc.toJson(QJsonDocument::Compact));
    QJsonDocument ngDoc;
    ngDoc.setObject(ngObj);
    m_strNg = QString(ngDoc.toJson(QJsonDocument::Compact));
    //设置工位标定信息
    this->accept();
}

void Cfhs_StationsVirtualStandardize::onCancelButton_clicked()  //取消
{
    this->reject();
}
