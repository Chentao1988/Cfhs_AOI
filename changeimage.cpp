#include "changeimage.h"

ChangeImage::ChangeImage()
{

}

QPixmap ChangeImage::addPoint(QPixmap &map,QList<QPoint> point)
{
    QPixmap newmap(100,100);
    newmap = map;
    QPainter paint;
    int len =0;
    while(len<point.size()){
        if(paint.begin(&newmap)){
            QPen pen;
            pen.setWidth(30);
            pen.setColor(Qt::red);
            pen.setBrush(Qt::red);
            paint.setPen(pen);
            paint.drawEllipse(point[len].x()-2,point[len].y()-2,4,4);
            paint.end();
        }
        else {
            qDebug()<<"Can not Painter";
            return map;
        }
        len++;
    }
    return newmap;
}
