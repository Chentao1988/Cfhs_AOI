#ifndef CFHS_SHOWZOOMIMAGE_H
#define CFHS_SHOWZOOMIMAGE_H

#include <QWidget>
#include <QPointer>

class QLabel;
class Cfhs_ComboBox;
class QPaintEvent;
class QPixmap;


class Cfhs_ShowZoomImage : public QWidget
{
    Q_OBJECT
public:
    explicit Cfhs_ShowZoomImage(QWidget *parent = nullptr);
    ~Cfhs_ShowZoomImage();

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void sig_setMultiple(const int &mul);

public slots:
    void slot_showImage(const QPixmap &map);

private slots:
    void on_multipleCombo_currentText_changed(const QString& currentText);

private:
    QPointer<QLabel> m_multipleLabel;
    QPointer<Cfhs_ComboBox> m_multipleCombo;
    QPointer<QLabel> m_imageLabel;
    QPixmap save_map;
};

#endif // CFHS_SHOWZOOMIMAGE_H
