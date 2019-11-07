#ifndef EDIT_DOUBLE_H
#define EDIT_DOUBLE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QIntValidator>
class Edit_Double: public QWidget
{
     Q_OBJECT
public:
    Edit_Double(QWidget *parent = nullptr);
    ~Edit_Double();

    void Set_Left_Label(QString Left);
    void Set_Right_Label(QString Right);
    void Get_Point(QPoint &point);
    void Set_Point(const QPoint &point);
private:
    QLabel* label_right;
    QLabel* label_left;
    QLineEdit *Edit_Right;
    QLineEdit *Edit_Left;
public slots:
    void Change_Edit_Left();
    void Change_Edit_Right();
    void SetText(QString data1,QString data2);
};


#endif // EDIT_DOUBLE_H
