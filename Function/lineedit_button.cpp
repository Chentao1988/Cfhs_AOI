#include "lineedit_button.h"
#pragma execution_character_set("utf-8")

lineedit_button::lineedit_button(QString text,QWidget *parent) :
    QWidget(parent)
{
    resize(200,100);
    QHBoxLayout * layout_all = new QHBoxLayout();
    QHBoxLayout * layout = new QHBoxLayout();
    label_text = new QLabel(this);
    lineedit = new QLineEdit(this);
    QPushButton * button = new QPushButton(this);
    lineedit->setReadOnly(true);
    label_text->setText(text);
    button->setFixedSize(17,16);
    button->setStyleSheet("QPushButton{background-image: url(:/down_arrow.png);background-color:transparent;border:none}");
    layout->addWidget(button,0,Qt::AlignRight|Qt::AlignVCenter);
    layout->setContentsMargins(0,0,0,0);
    lineedit->setTextMargins(0,0,20,0);
    lineedit->setLayout(layout);
    layout_all->addWidget(label_text,0,Qt::AlignLeft);
    layout_all->addWidget(lineedit);
    setLayout(layout_all);
    connect(button,SIGNAL(clicked()),this,SLOT(button_click()));
}

lineedit_button::~lineedit_button()
{

}

void lineedit_button::button_click()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("请选择图片"), "", "Image(*.png *.jpg *.bmp)");
    lineedit->setText(file_path);
}

QString lineedit_button::Get_Text()
{
    return label_text->text();
}

void lineedit_button::Set_Text(QString text)
{
    label_text->setText(text);
}

QString lineedit_button::Get_Path() const
{
    return lineedit->text();
}

void lineedit_button::Set_Path(const QString &path)
{
    lineedit->setText(path);
}
