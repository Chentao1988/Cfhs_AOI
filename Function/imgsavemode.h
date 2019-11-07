#ifndef IMGSAVEMODE_H
#define IMGSAVEMODE_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QDebug>
#include <QPushButton>
class ImgSaveMode : public QDialog
{
    Q_OBJECT

public:
    explicit ImgSaveMode(QWidget *parent = nullptr);
    ~ImgSaveMode();

public slots:
    void Button_Yes();      //确定
    void Button_No();       //取消
    void Box_Artwork();     //保存原图
    void Box_Compress();    //是否压缩

    //参数1为是否保存原图  //参数2为是否保存压缩图
    void SetImg_bool(bool artwork,bool compress);
    void SetImg_artwork(bool artwork);
    void SetImg_compress(bool compress);

    void GetImg_bool(bool &artwork,bool &compress);
    void GetImg_artwork(bool &artwork);
    void GetImg_compress(bool &compress);
private:
    bool flg_save_artwork_img;  //是否保存原图
    bool flg_compress_img;      //是否保存压缩图
    QPushButton *button_yes;
    QPushButton *button_no;
    QCheckBox *ArtworkBox;
    QCheckBox *CompressBox;
};

#endif // IMGSAVEMODE_H
