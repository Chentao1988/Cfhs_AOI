#ifndef CFHS_FRAME_H
#define CFHS_FRAME_H

#include <QFrame>
#include <QHBoxLayout>

class Cfhs_Frame : public QFrame
{
public:
    Cfhs_Frame(QWidget *parent = nullptr);
    ~Cfhs_Frame();
    void setWidget(QWidget *widget);

private:
    QHBoxLayout* m_layout;
};

#endif // CFHS_FRAME_H
