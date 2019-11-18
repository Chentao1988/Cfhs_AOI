#ifndef IMG_SHOW_MANAGENT_H
#define IMG_SHOW_MANAGENT_H
#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "cfhs_combobox.h"
#include "switch_button.h"
#include "lineedit_button.h"
#include "cfhs_global.h"

class Img_Show_Managent :public QDialog
{
    Q_OBJECT
public:
    Img_Show_Managent(QWidget *parent = nullptr);
    ~Img_Show_Managent();
    int DialogShow();

private slots:
    void button_yes_click();
    void button_no_click();
    void algorithm_open_button_clicked();
    void station_open_button_clicked();
    void button_static_img_clicked();

private:
    switch_button *algorithm_open_button;
    switch_button *station_open_button;
    QPushButton *button_yes;
    QPushButton *button_no;
    QPushButton *button_static_img;
    stConfig stConf;
};

#endif // IMG_SHOW_MANAGENT_H
