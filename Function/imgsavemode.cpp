#include "imgsavemode.h"
#pragma execution_character_set("utf-8")


ImgSaveMode::ImgSaveMode(QWidget *parent) :
    QDialog(parent)
{
    resize(400,200);
    this->setWindowTitle(tr("图片保存"));
    button_yes = new QPushButton(this);
    button_no = new QPushButton(this);
    ArtworkBox = new QCheckBox(this);
    CompressBox = new QCheckBox(this);
    button_yes->setText(tr("确定"));
    button_no->setText(tr("取消"));
    ArtworkBox->setText(tr("保存原图"));
    CompressBox->setText(tr("是否压缩"));
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *layout1 = new QHBoxLayout();
    layout->addWidget(ArtworkBox, 0, Qt::AlignCenter);
    layout->addWidget(CompressBox, 0, Qt::AlignCenter);
    layout1->addWidget(button_yes, 0, Qt::AlignCenter);
    layout1->addWidget(button_no, 0, Qt::AlignCenter);
    layout->addLayout(layout1);
    layout->setContentsMargins(9,9,9,20);
    setLayout(layout);
    connect(button_yes, SIGNAL(clicked()), this, SLOT(Button_Yes()));
    connect(button_no, SIGNAL(clicked()), this, SLOT(Button_No()));
    connect(ArtworkBox, SIGNAL(clicked()), this, SLOT(Box_Artwork()));
    connect(CompressBox, SIGNAL(clicked()), this, SLOT(Box_Compress()));

}

ImgSaveMode::~ImgSaveMode()
{

}

void ImgSaveMode::Button_Yes()
{
    qDebug()<<"Button_Yes";
    this->accept();
}

void ImgSaveMode::Button_No()
{
    qDebug()<<"Button_No";
    this->reject();
}

void ImgSaveMode::Box_Artwork()
{
    qDebug()<<"Artwork  "<<flg_save_artwork_img;
    flg_save_artwork_img = !flg_save_artwork_img;
}

void ImgSaveMode::Box_Compress()
{
    qDebug()<<"Compress  "<<flg_compress_img;
    flg_compress_img = !flg_compress_img;
}

void ImgSaveMode::SetImg_bool(bool artwork, bool compress)
{
    flg_compress_img = artwork;
    ArtworkBox->setChecked(artwork);
    flg_save_artwork_img = compress;
    CompressBox->setChecked(compress);

}

void ImgSaveMode::SetImg_artwork(bool artwork)
{
    flg_compress_img = artwork;
}

void ImgSaveMode::SetImg_compress(bool compress)
{
    flg_save_artwork_img = compress;
}

void ImgSaveMode::GetImg_bool(bool &artwork, bool &compress)
{
    artwork = flg_compress_img;
    compress = flg_save_artwork_img;
}



void ImgSaveMode::GetImg_artwork(bool &artwork)
{
    artwork = flg_compress_img;
}

void ImgSaveMode::GetImg_compress(bool &compress)
{
    compress = flg_save_artwork_img;
}
