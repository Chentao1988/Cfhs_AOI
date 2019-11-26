#include "cfhs_showzoomimage.h"
#include "cfhs_combobox.h"
#include "cfhs_base.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QHBoxLayout>


Cfhs_ShowZoomImage::Cfhs_ShowZoomImage(QWidget *parent)
    : QWidget(parent)
{
    //显示倍数调整
    m_multipleLabel = new QLabel(this);
    m_multipleLabel->setText(tr("选择放大比率："));
    m_multipleCombo = new Cfhs_ComboBox(this);
    m_multipleCombo->addItem("2X");
    m_multipleCombo->addItem("5X");
    m_multipleCombo->addItem("10X");
    m_multipleCombo->setCurrentText("5X");
    connect(m_multipleCombo, &Cfhs_ComboBox::currentTextChanged,
            this, &Cfhs_ShowZoomImage::on_multipleCombo_currentText_changed);
    QPointer<QHBoxLayout> topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(m_multipleLabel);
    topLayout->addWidget(m_multipleCombo);
    topLayout->setSpacing(15);
    topLayout->setContentsMargins(0,0,20,0);
    //图片label
    m_imageLabel = new QLabel(this);
    //layout
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_imageLabel, 1);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);
    this->resize(500, 400);
}

Cfhs_ShowZoomImage::~Cfhs_ShowZoomImage()
{

}

void Cfhs_ShowZoomImage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(save_map.isNull())
        return;
    QPainter painter(this);
    QRect picRect;
    int min_lod;
    int xPos = 0;
    int yPos = this->height() - m_imageLabel->height();
    int width = m_imageLabel->width();
    int height = m_imageLabel->height();

    if(save_map.width()-save_map.height()<0){
        min_lod = save_map.width()*height/save_map.height();
        picRect = QRect((width - min_lod)/2, yPos, min_lod, height);

    }
    else {
        min_lod = save_map.height()*width/save_map.width();
        picRect = QRect(xPos, (height - min_lod)/2, width, min_lod);
    }
    painter.drawPixmap(picRect, save_map);
}

void Cfhs_ShowZoomImage::slot_showImage(const QPixmap &map)
{
    save_map = map;
    this->update();
}

void Cfhs_ShowZoomImage::on_multipleCombo_currentText_changed(const QString &currentText)
{
    int multiple = currentText.section("X",0,0).toInt();
    emit sig_setMultiple(multiple);
}
