#include "cfhs_filterngdialog.h"
#include "cfhs_defectinfotable.h"
#include "cfhs_global.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringList>
#include <QMessageBox>


Cfhs_FilterNgDialog::Cfhs_FilterNgDialog(const QStringList &featureList, QWidget *parent)
    : QDialog (parent)
{
    //NG筛选表
    m_ngTable = new Cfhs_DefectInfoTable(tr("NG筛选"), featureList, false, this);
    m_ngTable->setHeadLabel(2, tr("NG判定方式"));
    //确定
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("确定"));
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_FilterNgDialog::on_commitButton_clicked);
    //取消
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, &QPushButton::clicked,
            this, &Cfhs_FilterNgDialog::on_cancelButton_clicked);
    QPointer<QHBoxLayout> bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_cancelButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_commitButton);
    bottomLayout->addStretch();
    //主layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_ngTable, 1);
    mainLayout->addLayout(bottomLayout);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10, 10, 10, 20);
    this->setLayout(mainLayout);
    this->setWindowTitle(tr("NG筛选"));
    this->resize(800, 600);
}

Cfhs_FilterNgDialog::~Cfhs_FilterNgDialog()
{

}

bool Cfhs_FilterNgDialog::setNgFilterInfo(const QString &info)
{
    //转成Map
    QMap<QString, QString> map;
    QString strInfo;  //日志
    bool isMap = getMapFromJson(info, map, strInfo);
    if(!isMap)
    {
        QMessageBox::warning(this, " ", strInfo);
        return false;
    }
    if(map.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("该方案的NG筛选数据为空"));
        return false;
    }
    //插入到表格中
    m_ngTable->setInfoMap(map);

    return true;
}

QString Cfhs_FilterNgDialog::getNgFilterInfo() const
{
    return m_strNgInfo;
}

void Cfhs_FilterNgDialog::on_commitButton_clicked()
{
    QMap<QString, QString> map;
    m_ngTable->getInfoMap(map);
    m_strNgInfo = getJsonFromMap(map);

    this->accept();
}

void Cfhs_FilterNgDialog::on_cancelButton_clicked()
{
    this->reject();
}
