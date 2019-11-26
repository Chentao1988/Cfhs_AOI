#include "cfhs_addcamerainfo.h"
#include "cfhs_combobox.h"
#include "Function/lineedit_button.h"
#include "cfhs_base.h"
#include "cfhs_global.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>


Cfhs_AddCameraInfo::Cfhs_AddCameraInfo(QWidget *parent)
    : QDialog (parent)
{
    //相机品牌
    m_cameraBrandLabel = new QLabel(this);
    m_cameraBrandLabel->setText(tr("相机品牌"));
    m_cameraBrandCombo = new Cfhs_ComboBox(this);
    m_cameraBrandCombo->setEditable(true);
    connect(m_cameraBrandCombo, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(on_cameraBrandCombo_currentIndex_changed(const QString&)));
    connect(m_cameraBrandCombo, &QComboBox::editTextChanged,
            this, &Cfhs_AddCameraInfo::on_cameraBrandCombo_eidtText_changed);
    QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
    topLayout->addWidget(m_cameraBrandLabel);
    topLayout->addWidget(m_cameraBrandCombo, 1);
    //相机型号
    m_cameraTypeLabel = new QLabel(this);
    m_cameraTypeLabel->setText(tr("相机型号"));
    m_cameraTypeCombo = new Cfhs_ComboBox(this);
    m_cameraTypeCombo->setEditable(true);
    QPointer<QHBoxLayout> bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_cameraTypeLabel);
    bottomLayout->addWidget(m_cameraTypeCombo, 1);
    //CCF文件
    m_ccfLineEdit = new lineedit_button(tr("CCF配置文件"), this, lineedit_button::FileCcf);
    //确定
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    m_commitButton->setDefault(true);
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_AddCameraInfo::on_commitButton_clicked);
    //取消
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_AddCameraInfo::on_cancelButton_clicked);
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_commitButton);
    buttonLayout->addStretch();
    //全局layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addWidget(m_ccfLineEdit);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(10,10,10,20);
    this->setLayout(mainLayout);
    this->setWindowTitle(tr("添加相机记录"));
    this->resize(400, 220);
}

Cfhs_AddCameraInfo::~Cfhs_AddCameraInfo()
{

}

int Cfhs_AddCameraInfo::DialogShow()
{
    //获取当前相机信息
    QString strAllCamera;
    QString strInfo;
    if(!m_logicInterface->GetCameraInfo(strAllCamera, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return QDialog::Rejected;
    }
    if(strAllCamera.isEmpty() || strAllCamera == "null")
        return this->exec();

    QStringList list = strAllCamera.split("@");
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
        QStringList listCcf;
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
    //插入相机品牌
    QStringList listName = getKeyListFromMap(m_mapCamera);
    m_cameraBrandCombo->clear();
    m_cameraBrandCombo->addItems(listName);
    m_cameraBrandCombo->setCurrentIndex(0);

    return this->exec();
}

QStringList Cfhs_AddCameraInfo::getKeyListFromMap(const QMap<QString, QStringList> &map)
{
    if(map.isEmpty())
        return QStringList();

    QStringList list;
    QMap<QString, QStringList>::const_iterator iter = map.begin();
    while(iter != map.end())
    {
        list.append(iter.key());
        iter++;
    }

    return list;
}

QStringList Cfhs_AddCameraInfo::getKeyListFromMap(const QMap<QString, QMap<QString, QStringList>> &map)
{
    if(map.isEmpty())
        return QStringList();

    QStringList list;
    QMap<QString, QMap<QString, QStringList>>::const_iterator iter = map.begin();
    while(iter != map.end())
    {
        list.append(iter.key());
        iter++;
    }

    return list;
}

void Cfhs_AddCameraInfo::on_cameraBrandCombo_currentIndex_changed(const QString &text)
{
    if(m_mapCamera.contains(text))
    {
        QMap<QString, QStringList> map = m_mapCamera.value(text);
        m_cameraTypeCombo->clear();
        QStringList listType = getKeyListFromMap(map);
        m_cameraTypeCombo->addItems(listType);
        m_cameraTypeCombo->setCurrentIndex(0);
    }
}

void Cfhs_AddCameraInfo::on_cameraBrandCombo_eidtText_changed(const QString &text)
{
    Q_UNUSED(text);
    m_cameraTypeCombo->clear();
}

void Cfhs_AddCameraInfo::on_commitButton_clicked()
{
    QString strBrand = m_cameraBrandCombo->currentText();
    if(strBrand.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("相机品牌不能为空"));
        return;
    }
    QString strType = m_cameraTypeCombo->currentText();
    if(strType.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("相机型号不能为空"));
        return;
    }
    QString strCcf = m_ccfLineEdit->Get_Path();
    if(strCcf.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("CCF配置文件不能为空"));
        return;
    }
    //检查该记录是否已存在
    bool isExisted = false;
    if(m_mapCamera.contains(strBrand))
    {
        if(m_mapCamera.value(strBrand).contains(strType))
        {
            if(m_mapCamera.value(strBrand).value(strType).contains(strCcf))
                isExisted = true;
        }
    }
    if(isExisted)
    {
        QMessageBox::warning(this, " ", tr("该记录已存在，请重新设置"));
        return;
    }
#if 1
    stCameraInfo stCamera;
    stCamera.strCameraType = strBrand;
    stCamera.strCameraSpecification = strType;
    stCamera.strCcfFilePath = strCcf;
    QString strInfo;
    if(!m_logicInterface->InsertCameraInfo(stCamera, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
#endif
    this->accept();
}

void Cfhs_AddCameraInfo::on_cancelButton_clicked()
{
    this->reject();
}
