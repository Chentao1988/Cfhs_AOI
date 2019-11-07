#ifndef IMGSUBSET_H
#define IMGSUBSET_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QDebug>
#include <QLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QLineEdit>
#include "switch_button.h"
#include "edit_double.h"
class ImgSubSet : public QDialog
{
    Q_OBJECT

public:
    explicit ImgSubSet(QWidget *parent = nullptr);
    ~ImgSubSet();
    void Set_Point(QPoint left_top,QPoint right_top,
                   QPoint left_bottom,QPoint right_bottom,
                   bool flg_alphabet);
    void Set_Grid_View(bool open_gridview,int x_gridview,int y_gridview);

signals:
    void sig_setPoint(QPoint left_top,QPoint right_top,
                      QPoint left_bottom,QPoint right_bottom,
                      bool flg_alphabet);
    void sig_setGridView(bool open_gridview,int x_gridview,int y_gridview);

private slots:
    void button_cancel_clicked();
    void button_detemine_clicked();
    void negate_button_clicked();
    void open_button_clicked();

private:
    QPushButton *button_cancel;
    QPushButton *button_detemine;
    Edit_Double *edit_PointA;
    Edit_Double *edit_PointB;
    Edit_Double *edit_PointC;
    Edit_Double *edit_PointD;
    Edit_Double *edit_Partition;
    switch_button *negate_button;
    switch_button *open_button;
};

#endif // IMGSUBSET_H
