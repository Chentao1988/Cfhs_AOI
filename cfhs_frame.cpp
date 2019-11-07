#include "cfhs_frame.h"

Cfhs_Frame::Cfhs_Frame(QWidget* parent)
    : QFrame(parent)
{
    m_layout = new QHBoxLayout();
    m_layout->setContentsMargins(1,1,1,1);
    this->setLayout(m_layout);
}

Cfhs_Frame::~Cfhs_Frame()
{

}

void Cfhs_Frame::setWidget(QWidget *widget)
{
    m_layout->addWidget(widget);
}
