#include "cfhs_imagetranscoding.h"
#include "lineedit_button.h"
#include "../cfhs_global.h"
#include "opencv2/opencv.hpp"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <QFile>


Cfhs_ImageTranscoding::Cfhs_ImageTranscoding(QWidget *parent, const int &stationNum)
    : QDialog (parent)
{
    QVBoxLayout *main_layout = new QVBoxLayout;
    //结果图
    result_img = new lineedit_button(tr("结果图"), this);
    main_layout->addWidget(result_img);
    //分析图
    analysis_img = new lineedit_button(tr("分析图"), this);
    main_layout->addWidget(analysis_img);
    //工位图
    for(int i=0; i<stationNum; i++)
    {
        int stationNo = i + 1;
        QString strStation = QString(tr("工位%1图")).arg(stationNo);
        lineedit_button *line_edit = new lineedit_button(strStation, this);
        map_station.insert(stationNo, line_edit);
        main_layout->addWidget(line_edit);
    }
    //commit button
    button_commit = new QPushButton(this);
    button_commit->setText(tr("确定"));
    button_commit->setDefault(true);
    //cancel button
    button_cancel = new QPushButton(this);
    button_cancel->setText(tr("取消"));
    QHBoxLayout *button_layout = new QHBoxLayout;
    button_layout->addWidget(button_cancel, 0, Qt::AlignCenter);
    button_layout->addWidget(button_commit, 0, Qt::AlignCenter);
    main_layout->addLayout(button_layout);
    main_layout->setSpacing(20);
    this->setLayout(main_layout);
    this->resize(500, 200+60*stationNum);
    this->setWindowTitle(tr("设置工位静态图"));
    //信号槽连接
    connect(button_commit, &QPushButton::clicked,
            this, &Cfhs_ImageTranscoding::button_commit_clicked);
    connect(button_cancel, &QPushButton::clicked,
            this, &Cfhs_ImageTranscoding::button_cancel_clicked);
}

Cfhs_ImageTranscoding::~Cfhs_ImageTranscoding()
{


}

bool Cfhs_ImageTranscoding::isStaticImgExists()
{
    QDir dir;
    //文件目录
    QString strDir = QString("D:/%1/static_img").arg(m_curProgramName);
    if(!dir.exists(strDir))
    {
        QMessageBox::warning(nullptr, " ", tr("工位静态图不存在，请设置"));
        return false;
    }
    QFile file;
    //结果图
    QString result_path = QString("%1/%2").arg(strDir).arg("result.jpg");
    if(!file.exists(result_path))
    {
        QMessageBox::warning(nullptr, " ", tr("结果图不存在，请设置"));
        return false;
    }
    //分析图
    QString analysis_path = QString("%1/%2").arg(strDir).arg("analysis.jpg");
    if(!file.exists(analysis_path))
    {
        QMessageBox::warning(nullptr, " ", tr("分析图不存在，请设置"));
        return false;
    }
    //工位图
    for(int i=0; i<4; i++)
    {
        int stationNo = i + 1;
        QString station_path = QString("%1/station%2.jpg").arg(strDir).arg(stationNo);
        if(!file.exists(station_path))
        {
            QString strInfo = QString(tr("工位%1图不存在，请设置")).arg(stationNo);
            QMessageBox::warning(nullptr, " ", strInfo);
            return false;
        }
    }

    return true;
}

bool Cfhs_ImageTranscoding::setImageTranscode(const QString &station_name,
                                              const QString &image_path,
                                              QString &strInfo)
{
    //文件目录
    QString strDir = QString("D:/%1/static_img").arg(m_curProgramName);
    QDir dir;
    if(!dir.exists(strDir))
    {
        //不存在则创建目录
        if(!dir.mkpath(strDir))
        {
            strInfo = QString(tr("目录(%1)创建失败")).arg(strDir);
            return false;
        }
    }
    //要保存的文件名
    QString strSave = QString("%1/%2.jpg").arg(strDir).arg(station_name);
    cv::Mat image = cv::imread(getStringFromQString(image_path), -1);
    cv::imwrite(getStringFromQString(strSave), image);
    return true;
}

bool Cfhs_ImageTranscoding::isImagePathValid()
{
    //结果图
    QString result_path = result_img->Get_Path();
    if(result_path.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请选择结果图"));
        return false;
    }
    //分析图
    QString analysis_path = analysis_img->Get_Path();
    if(analysis_path.isEmpty())
    {
        QMessageBox::warning(this, " ", tr("请选择分析图"));
        return false;
    }
    //工位图
    QMap<int, lineedit_button*>::const_iterator iter = map_station.begin();
    while(iter != map_station.end())
    {
        QString station_path = iter.value()->Get_Path();
        QString strInfo;
        if(station_path.isEmpty())
        {
            strInfo = QString(tr("请选择工位%1图")).arg(iter.key());
            QMessageBox::warning(this, " ", strInfo);
            return false;
        }
        iter++;
    }

    return true;
}

std::string Cfhs_ImageTranscoding::getStringFromQString(const QString &info)
{
    QByteArray buf = info.toLocal8Bit();
    std::string string;
    string.append(buf);

    return string;
}

void Cfhs_ImageTranscoding::button_commit_clicked()
{
    if(!isImagePathValid())
        return;
    QMessageBox *msg = new QMessageBox(QMessageBox::Information,
                                        tr("提示"),
                                        tr("图片正在保存中，请稍后..."),
                                        QMessageBox::Yes,
                                        this);
    msg->show();
    Sleep(10);
    QString strInfo;
    //结果图
    QString result_path = result_img->Get_Path();
    if(!setImageTranscode("result", result_path, strInfo))
    {
        msg->close();
        strInfo = QString(tr("结果图设置失败：%1")).arg(strInfo);
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //分析图
    QString analysis_path = analysis_img->Get_Path();
    if(!setImageTranscode("analysis", analysis_path, strInfo))
    {
        msg->close();
        strInfo = QString(tr("分析图设置失败：%1")).arg(strInfo);
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    //工位图
    QMap<int, lineedit_button*>::const_iterator iter = map_station.begin();
    while(iter != map_station.end())
    {
        QString strStation = QString("station%1").arg(iter.key());
        QString station_path = iter.value()->Get_Path();
        if(!setImageTranscode(strStation, station_path, strInfo))
        {
            msg->close();
            strInfo = QString(tr("工位%1图设置失败：%2")).arg(iter.key()).arg(strInfo);
            QMessageBox::warning(this, " ", strInfo);
            return;
        }

        iter++;
    }
    msg->close();
    delete msg;
    msg = nullptr;
    this->accept();
}

void Cfhs_ImageTranscoding::button_cancel_clicked()
{
    this->reject();
}
