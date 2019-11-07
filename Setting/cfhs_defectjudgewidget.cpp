#include "cfhs_defectjudgewidget.h"
#include "cfhs_base.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>


Cfhs_DefectJudgeWidget::Cfhs_DefectJudgeWidget(const QString& winTitle,
                                               const QStringList& nameList,
                                               QWidget *parent)
    : QDialog (parent)
{
    //添加button
    m_addButton = new QPushButton(this);
    m_addButton->setText(tr("添加"));
    connect(m_addButton, &QPushButton::clicked, this, &Cfhs_DefectJudgeWidget::onAddButton_clicked);
    //插入button
    m_insertButton = new QPushButton(this);
    m_insertButton->setText(tr("插入"));
    connect(m_insertButton, &QPushButton::clicked, this, &Cfhs_DefectJudgeWidget::onInsertButton_clicked);
    //删除button
    m_delButton = new QPushButton(this);
    m_delButton->setText(tr("删除"));
    connect(m_delButton, &QPushButton::clicked, this, &Cfhs_DefectJudgeWidget::onDelButton_clicked);
    //保存button
    m_saveButton = new QPushButton(this);
    m_saveButton->setText(tr("保存"));
    connect(m_saveButton, &QPushButton::clicked, this, &Cfhs_DefectJudgeWidget::onSaveButton_clicked);
    //添加button layout
    QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_insertButton);
    buttonLayout->addWidget(m_delButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addStretch();
    buttonLayout->setSpacing(30);
    //缺陷判定方式编辑表
    m_defectTable = new Cfhs_DefectJudgeTable(nameList, this);
    //添加主界面layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout, 0);
    mainLayout->addWidget(m_defectTable, 1);
    mainLayout->setSpacing(10);
    this->setLayout(mainLayout);
    //主界面设置
    this->setWindowTitle(winTitle);
    this->setWindowFlags(this->windowFlags()|Qt::WindowMinMaxButtonsHint);
    this->resize(1200, 400);
}

Cfhs_DefectJudgeWidget::~Cfhs_DefectJudgeWidget()
{

}

void Cfhs_DefectJudgeWidget::onAddButton_clicked()
{
    m_defectTable->addOneRow();
}

void Cfhs_DefectJudgeWidget::onInsertButton_clicked()
{
    int curRow = m_defectTable->currentRow();
    if(curRow < 0)
    {
        QMessageBox::warning(this, tr("提示"), tr("请选择要插入的行"));
        return;
    }
    QString info = QString(tr("是否在第%1行的位置插入一行？")).arg(curRow+1);
    if(QMessageBox::Yes == QMessageBox::information(this,tr("提示"),info,QMessageBox::Yes|QMessageBox::No))
        m_defectTable->insetOneRow(curRow);
}

void Cfhs_DefectJudgeWidget::onDelButton_clicked()
{
    int curRow = m_defectTable->currentRow();
    if(curRow < 0)
    {
        QMessageBox::warning(this, tr("提示"), tr("请选择要删除的行"));
        return;
    }
    QString info = QString(tr("是否删除第%1行？")).arg(curRow+1);
    if(QMessageBox::Yes == QMessageBox::information(this,tr("提示"),info,QMessageBox::Yes|QMessageBox::No))
        m_defectTable->delOneRow(curRow);
}

void Cfhs_DefectJudgeWidget::onSaveButton_clicked()
{
    QString defectInfo;
    if(m_defectTable->saveData(defectInfo))
    {
        emit sendDefectInfo(defectInfo);
        this->accept();
    }
}

