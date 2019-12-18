#include "cfhs_sidetool.h"
#include "cfhs_base.h"
#include "cfhs_global.h"
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QFrame>
#include <QFileDialog>
#include <QMessageBox>


Cfhs_SideTool::Cfhs_SideTool(QWidget *parent)
    : QWidget (parent)
{
    //显示图标
    m_showButton = new QPushButton(this);
    m_showButton->setFixedSize(QSize(24, 24));
    connect(m_showButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_showButton_clicked);
    //离线模式
    m_offlineButton = this->addButton(tr("离线模式"), "offline");
    connect(m_offlineButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_offlineButton_clicked);
    //导入图片
    m_importImageButton = this->addButton(tr("导入图片"), "import");
    connect(m_importImageButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_importImageButton_clicked);
    //导出图片
    m_exportImageButton = this->addButton(tr("导出图片"), "export");
    connect(m_exportImageButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_exportImageButton_clicked);
    //复测执行
    m_retestButton = this->addButton(tr("复测执行"), "retest");
    connect(m_retestButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_retestButton_clicked);
    //上一张
    m_prevImageButton = this->addButton(tr("上一张"), "prev_image");
    connect(m_prevImageButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_prevImageButton_clicked);
    //下一张
    m_nextImageButton = this->addButton(tr("下一张"), "next_image");
    connect(m_nextImageButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_nextImageButton_clicked);
    //循环模式
    m_cycleButton = this->addButton(tr("循环模式"), "cycle");
    connect(m_cycleButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_cycleButton_clicked);
    //查找产品ID图
    m_findIDButton = this->addButton(tr("查找产品ID图"), "find_product");
    connect(m_findIDButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_findIDButton_clicked);
    //下载文件区域
    m_loadFileAreaButton = this->addButton(tr("下载文件区域"), "upload");
    connect(m_loadFileAreaButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_loadFileAreaButton_clicked);
    //报表查询导出
    m_reportQueryButton = this->addButton(tr("报表查询导出"), "find_report");
    connect(m_reportQueryButton, &QPushButton::clicked,
            this, &Cfhs_SideTool::on_reportQueryButton_clicked);
    //添加layout
    QPointer<QVBoxLayout> rightLayout = new QVBoxLayout();
    rightLayout->addWidget(m_offlineButton);
    rightLayout->addWidget(m_importImageButton);
    rightLayout->addWidget(m_exportImageButton);
    rightLayout->addWidget(m_retestButton);
    rightLayout->addWidget(m_prevImageButton);
    rightLayout->addWidget(m_nextImageButton);
    rightLayout->addWidget(m_cycleButton);
    rightLayout->addWidget(m_findIDButton);
    rightLayout->addWidget(m_loadFileAreaButton);
    rightLayout->addWidget(m_reportQueryButton);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    //buttonFrame
    m_buttonFrame = new QFrame(this);
    m_buttonFrame->setLayout(rightLayout);
    //添加主layout
    QPointer<QHBoxLayout> mainLayout = new QHBoxLayout();
    mainLayout->addWidget(m_showButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_buttonFrame, 1);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);
    //默认关闭
    m_isOfflineOpened = false;
    setSideShow(false);
}

Cfhs_SideTool::~Cfhs_SideTool()
{

}

void Cfhs_SideTool::setSideEnable(const bool &isEnable)
{
    m_showButton->setEnabled(isEnable);
    setSideShow(isEnable);
}

void Cfhs_SideTool::setSideShow(const bool &isShow)
{
    QString style = "";
    if(isShow)
    {
        style = QString("QPushButton{border-image:url(:/side_show.png);"
                        "background:transparent; border:none;}");
    }
    else
    {
        style = QString("QPushButton{border-image:url(:/side_hide.png);"
                        "background:transparent; border:none;}");
    }
    m_showButton->setStyleSheet(style);
    m_buttonFrame->setVisible(isShow);
    m_isShowed = isShow;
    if(!m_isShowed && m_isOfflineOpened)
    {
        //退出离线模式
        m_logicInterface->OffLineExit();
        m_isOfflineOpened = false;
    }
}

bool Cfhs_SideTool::isSideShow() const
{
    return m_isShowed;
}

QPushButton* Cfhs_SideTool::addButton(const QString& title, const QString& iconName)
{
    QPushButton *button = new QPushButton(this);
    button->setToolTip(title);
    button->setFixedSize(60, 60);
    QString style = QString("QPushButton{border-image: url(:/%1_normal.png);"
                            "background:transparent;border:none;}"
                            "QPushButton:pressed{border-image: url(:/%2_press.png)}")
            .arg(iconName)
            .arg(iconName);
    button->setStyleSheet(style);

    return button;
}

QString Cfhs_SideTool::getDirFromFilePath(const QString &file)
{
#if 1
    int lastNo = file.lastIndexOf("/");
    QString newFile = file.mid(0, lastNo);
#else
    QStringList list = file.split("/");
    QString newFile;
    //去掉最后的文件名
    for(int i=0; i<list.size()-1; i++)
    {
        newFile.append(list.at(i));
        if(i < list.size()-2)
            newFile.append("/");
    }
#endif
    return newFile;
}

void Cfhs_SideTool::on_offlineButton_clicked()
{
    m_offlineButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    m_logicInterface->OffLineInit();
    m_isOfflineOpened = true;
    m_offlineButton->setEnabled(true);
}

void Cfhs_SideTool::on_importImageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("导入图片"), "", "Image(*.bmp *.png)");
    if(filePath.isEmpty())
        return;
    QString dir = getDirFromFilePath(filePath);
    m_importImageButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    //发送信号
    m_logicInterface->OffLineInImg(dir);
    m_importImageButton->setEnabled(true);
}

void Cfhs_SideTool::on_exportImageButton_clicked()
{
    m_exportImageButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    m_logicInterface->OffLineOutImg();
    m_exportImageButton->setEnabled(true);
}

void Cfhs_SideTool::on_retestButton_clicked()
{
    m_retestButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    m_logicInterface->OffLineOne();
    m_retestButton->setEnabled(true);
}

void Cfhs_SideTool::on_prevImageButton_clicked()
{
    m_prevImageButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    m_logicInterface->OffLineLast();
    m_prevImageButton->setEnabled(true);
}

void Cfhs_SideTool::on_nextImageButton_clicked()
{
    m_nextImageButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    m_logicInterface->OffLineNext();
    m_nextImageButton->setEnabled(true);
}

void Cfhs_SideTool::on_cycleButton_clicked()
{
    m_cycleButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    m_logicInterface->OffLineLoop();
    m_cycleButton->setEnabled(true);
}

void Cfhs_SideTool::on_findIDButton_clicked()
{

}

void Cfhs_SideTool::on_loadFileAreaButton_clicked()
{
#if 0
    QString dir = QFileDialog::getOpenFileName(this, tr("文件下载区域"), "", "Image(*.bmp *.png)");
    if(dir.isEmpty())
        return;
    dir = getDirFromFilePath(dir);
#else
    QString dir = QFileDialog::getExistingDirectory(this, tr("文件下载区域"), "");
    if(dir.isEmpty())
        return;
#endif
    m_loadFileAreaButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    //发送信号
    m_logicInterface->OffLineDownFile(dir);
    m_loadFileAreaButton->setEnabled(true);
}

void Cfhs_SideTool::on_reportQueryButton_clicked()
{
    m_reportQueryButton->setEnabled(false);
    ShowMessage(tr("命令执行中，请稍后..."));
    m_logicInterface->OffLineReport();
    m_reportQueryButton->setEnabled(true);
}

void Cfhs_SideTool::on_showButton_clicked()
{
    bool isShow = !m_isShowed;
    setSideShow(isShow);
}
