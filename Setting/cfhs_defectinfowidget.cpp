#include "cfhs_defectinfowidget.h"
#include "cfhs_base.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <iterator>


Cfhs_DefectInfoWidget::Cfhs_DefectInfoWidget(const QStringList &listFeather, QWidget *parent)
    : QWidget(parent)
{
    init(listFeather);
}

Cfhs_DefectInfoWidget::Cfhs_DefectInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    init();
}

Cfhs_DefectInfoWidget::~Cfhs_DefectInfoWidget()
{

}

QMap<QString, QString> Cfhs_DefectInfoWidget::getDefectInfoMap()
{
    QMap<QString, QString> map;
    m_infoTable->getInfoMap(map);
    return map;
}

void Cfhs_DefectInfoWidget::setDefectInfoMap(const QMap<QString, QString> &map)
{
    m_infoTable->setInfoMap(map);
}

void Cfhs_DefectInfoWidget::setFeatherList(const QStringList &list)
{
    m_infoTable->setFeatureList(list);
}

void Cfhs_DefectInfoWidget::init(const QStringList &listFeather)
{
    //缺陷名称
    m_defectTitleLabel = new QLabel(this);
    m_defectTitleLabel->setText(tr("添加行："));
    //缺陷名编辑框
    m_defectNameLineEdit = new QLineEdit(this);
    m_defectNameLineEdit->setPlaceholderText(tr("输入缺陷名称"));
    //确定button
    m_commitButton = new QPushButton(this);
    m_commitButton->setText(tr("添加"));
    connect(m_commitButton, &QPushButton::clicked,
            this, &Cfhs_DefectInfoWidget::onCommitButton_clicked);
    //以上项添加layout
    QPointer<QHBoxLayout> topLayout = new QHBoxLayout();
    topLayout->addWidget(m_defectTitleLabel);
    topLayout->addWidget(m_defectNameLineEdit);
    topLayout->addWidget(m_commitButton);
    topLayout->addStretch();
    topLayout->setSpacing(10);
    //缺陷信息表
    m_infoTable = new Cfhs_DefectInfoTable(tr("工位缺陷定义"), listFeather, true, this);
    connect(m_infoTable, &Cfhs_DefectInfoTable::sig_editOneRow,
            this, &Cfhs_DefectInfoWidget::sig_editOneRow);
    connect(m_infoTable, &Cfhs_DefectInfoTable::sig_deleteOneRow,
            this, &Cfhs_DefectInfoWidget::sig_deleteOneRow);
    //添加整体layout
    QPointer<QVBoxLayout> layout = new QVBoxLayout();
    layout->addLayout(topLayout, 0);
    layout->addWidget(m_infoTable, 1);
    layout->setSpacing(10);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);
}

void Cfhs_DefectInfoWidget::onCommitButton_clicked()
{
    QString name = m_defectNameLineEdit->text();
    if(name.isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("请输入缺陷名"));
        return;
    }
    if(m_infoTable->addOneRow(name))
    {
        m_defectNameLineEdit->setText("");
        emit sig_addOneRow(name);
    }
}
