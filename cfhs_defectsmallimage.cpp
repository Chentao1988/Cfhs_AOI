#include "cfhs_defectsmallimage.h"
#include "cfhs_base.h"
#include <QVBoxLayout>
#include <QPixmap>

Cfhs_DefectSmallImage::Cfhs_DefectSmallImage(QWidget *parent)
    : QWidget(parent)
{
    m_defectImageLabel = new QLabel(this);
    //m_defectInfoLabel->setStyleSheet("image:none");
    m_defectInfoLabel = new QLabel(this);
    m_defectInfoLabel->setStyleSheet("color: #2193AB");
    QPointer<QVBoxLayout> layout = new QVBoxLayout();
    layout->addWidget(m_defectImageLabel,1,Qt::AlignCenter);
    layout->addWidget(m_defectInfoLabel,0,Qt::AlignCenter);
    layout->setSpacing(20);
    this->setLayout(layout);
}

Cfhs_DefectSmallImage::~Cfhs_DefectSmallImage()
{

}

void Cfhs_DefectSmallImage::showDefect(const QImage& image, const QString& defectInfo)
{
    //显示缺陷图
    QPixmap map = QPixmap::fromImage(image);
    map = map.scaled(120,120,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    m_defectImageLabel->setPixmap(map);
    //显示缺陷信息
    m_defectInfoLabel->setText(defectInfo);
}

void Cfhs_DefectSmallImage::clearContent()
{
    m_defectInfoLabel->clear();
    m_defectImageLabel->clear();
}
