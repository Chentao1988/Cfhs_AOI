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
class Img_Show_Managent :public QDialog
{
    Q_OBJECT
public:
    Img_Show_Managent(int number = 0,QWidget *parent = nullptr);
    ~Img_Show_Managent();
    void DialogShow();
private:
    Cfhs_ComboBox *result_ComboBox;
    Cfhs_ComboBox *analysis_ComboBox;
    QPushButton *button_yes;
    QPushButton *button_no;
public slots:
    void Add_Station(QString station);
    void button_yes_click();
    void button_no_click();
    void result_open_button_clicked();
    void station_open_button_clicked();
private:
    lineedit_button *button_result;
    lineedit_button *button_analysis;
    switch_button *result_open_button;
    switch_button *station_open_button;
    QMap<int, lineedit_button*> map_station;
};

#endif // IMG_SHOW_MANAGENT_H
