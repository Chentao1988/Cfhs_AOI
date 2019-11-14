#ifndef IMGSAVEMODE_H
#define IMGSAVEMODE_H

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include "../cfhs_global.h"

class ImgSaveMode : public QDialog
{
    Q_OBJECT

public:
    explicit ImgSaveMode(QWidget *parent = nullptr);
    ~ImgSaveMode();
    void DialogShow();

private slots:
    void on_button_yes_clicked();      //确定
    void on_button_no_clicked();       //取消

private:
    QPushButton *button_yes;
    QPushButton *button_no;
    QCheckBox *save_original_ok;
    QCheckBox *save_original_ng;
    QCheckBox *save_compress_ok;
    QCheckBox *save_compress_ng;
    stConfig stConf;
};

#endif // IMGSAVEMODE_H
