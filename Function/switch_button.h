#ifndef SWITCH_BUTTON_H
#define SWITCH_BUTTON_H
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
class switch_button: public QWidget
{
     Q_OBJECT
public:
    switch_button(QString data1 = "",QString data2 = "",QWidget *parent = nullptr);
    ~switch_button();

    void Set_Left_Label(QString Left);
    void Set_Right_Label(QString Right);
    bool Get_Switch() const;
    void Set_Switch(const bool &isOpen);
    void Set_Switch_Enable(const bool &isEnable);
    static QString getSwitchButtonStyle(const bool &isSwitch);

signals:
    void sig_button_clicked();

private:
    QLabel* label_right;
    QLabel* label_left;
    QPushButton *button_center;
    bool Flg_LeftOrRight;
};

#endif // SWITCH_BUTTON_H
