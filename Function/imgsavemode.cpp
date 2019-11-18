#include "imgsavemode.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>


#pragma execution_character_set("utf-8")


ImgSaveMode::ImgSaveMode(QWidget *parent) :
    QDialog(parent)
{
    //确定
    button_yes = new QPushButton(this);
    button_yes->setText(tr("确定"));
    button_yes->setDefault(true);
    connect(button_yes, &QPushButton::clicked,
            this, &ImgSaveMode::on_button_yes_clicked);
    //取消
    button_no = new QPushButton(this);
    button_no->setText(tr("取消"));
    connect(button_no, &QPushButton::clicked,
            this, &ImgSaveMode::on_button_no_clicked);
    QHBoxLayout *button_layout = new QHBoxLayout;
    button_layout->addWidget(button_no, 0, Qt::AlignCenter);
    button_layout->addWidget(button_yes, 0, Qt::AlignCenter);
    //保存OK图
    save_original_ok = new QCheckBox(this);
    save_original_ok->setText(tr("保存OK原图"));
    save_compress_ok = new QCheckBox(this);
    save_compress_ok->setText(tr("保存OK压缩图"));
    QHBoxLayout *ok_layout = new QHBoxLayout;
    ok_layout->addWidget(save_original_ok, 0, Qt::AlignCenter);
    ok_layout->addWidget(save_compress_ok, 0, Qt::AlignCenter);
    //保存NG图
    save_original_ng = new QCheckBox(this);
    save_original_ng->setText(tr("保存NG原图"));
    save_compress_ng = new QCheckBox(this);
    save_compress_ng->setText(tr("保存NG压缩图"));
    QHBoxLayout *ng_layout = new QHBoxLayout;
    ng_layout->addWidget(save_original_ng, 0, Qt::AlignCenter);
    ng_layout->addWidget(save_compress_ng, 0, Qt::AlignCenter);
    //全局layout
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(ok_layout);
    main_layout->addLayout(ng_layout);
    main_layout->addLayout(button_layout);
    main_layout->setSpacing(20);
    this->setLayout(main_layout);
    this->resize(400,220);
    this->setWindowTitle(tr("图片保存"));
}

ImgSaveMode::~ImgSaveMode()
{

}

void ImgSaveMode::DialogShow()
{
    QString strInfo;
    if(!m_logicInterface->GetConfigInfo(stConf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    save_original_ok->setChecked(stConf.bSaveOKImg);
    save_compress_ok->setChecked(stConf.bSaveCompressedOKImg);
    save_original_ng->setChecked(stConf.bSaveNGImg);
    save_compress_ng->setChecked(stConf.bSaveCompressedNGImg);
    //工位图是动态图是固定压缩
    if(stConf.bStationDynamic)
    {
        save_compress_ok->setChecked(true);
        save_compress_ok->setEnabled(false);
        save_compress_ng->setChecked(true);
        save_compress_ng->setEnabled(false);
    }
    this->exec();
}

void ImgSaveMode::on_button_yes_clicked()
{
    stConf.bSaveOKImg = save_original_ok->isChecked();
    stConf.bSaveCompressedOKImg = save_compress_ok->isChecked();
    stConf.bSaveNGImg = save_original_ng->isChecked();
    stConf.bSaveCompressedNGImg = save_compress_ng->isChecked();
    QString strInfo;
    if(!m_logicInterface->SetConfigInfo(stConf, strInfo))
    {
        QMessageBox::warning(this, " ", strInfo);
        return;
    }
    QMessageBox::information(this, " ", tr("设置成功"));
    this->accept();
}

void ImgSaveMode::on_button_no_clicked()
{
    this->reject();
}
