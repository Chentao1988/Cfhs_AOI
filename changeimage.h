#ifndef CHANGEIMAGE_H
#define CHANGEIMAGE_H
#include<QPixmap>
#include<QPainter>
#include<QDebug>
class ChangeImage
{
public:
    ChangeImage();
    static QPixmap addPoint(QPixmap &map,QList<QPoint> point);
};

#endif // CHANGEIMAGE_H
