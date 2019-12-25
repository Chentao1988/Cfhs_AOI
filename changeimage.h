#ifndef CHANGEIMAGE_H
#define CHANGEIMAGE_H
#include<QPixmap>
#include<QPainter>
#include <QImage>
#include<QDebug>

enum ShapeType{Line, Rect, Ellipse};

typedef struct _itoPoint{
    QPoint Start;
    QPoint End;
} itoPoint;

class ChangeImage
{
public:
    ChangeImage();
    static QPixmap AddPoint_Draw(const QImage &img,QList<itoPoint>List_Point, const ShapeType &shape = Line);
    static QPixmap addPoint(QPixmap &map,QList<QPoint> point);
};

#endif // CHANGEIMAGE_H
