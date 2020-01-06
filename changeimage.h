#ifndef CHANGEIMAGE_H
#define CHANGEIMAGE_H
#include<QPixmap>
#include<QPainter>
#include <QImage>
#include<QDebug>

enum ShapeType{None,Line, Rect, Ellipse};

typedef struct _itoPoint{
    QPoint Start;
    QPoint End;
} itoPoint;

class ChangeImage
{
public:
    ChangeImage();
    QPixmap addPoint(const QPixmap &map,const QList<QPoint> point,bool pyr_flg = false);

    QPixmap AddPoint_Draw(const QImage &img,QList<itoPoint>List_Point, const ShapeType &shape = Line,bool pyr_flg = false);
};

#endif // CHANGEIMAGE_H
