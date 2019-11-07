#ifndef CFHS_DEFECTSMALLIMAGE_H
#define CFHS_DEFECTSMALLIMAGE_H

#include <QWidget>
#include <QLabel>
#include <QPointer>
#include <QImage>


//该类主要用于显示主界面上的缺陷小图，包括缺陷图和缺陷信息
class Cfhs_DefectSmallImage : public QWidget
{
    Q_OBJECT
public:
    explicit Cfhs_DefectSmallImage(QWidget *parent = nullptr);
    ~Cfhs_DefectSmallImage();
    /*******显示缺陷图片和信息
     * image  缺陷图
     * defectInfo  缺陷信息
     *****************************/
    void showDefect(const QImage& image, const QString& defectInfo);
    //清空数据
    void clearContent();

signals:

public slots:

private:
    QPointer<QLabel> m_defectImageLabel;
    QPointer<QLabel> m_defectInfoLabel;

};

#endif // CFHS_DEFECTSMALLIMAGE_H
