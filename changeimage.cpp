#include "changeimage.h"

ChangeImage::ChangeImage()
{

}

QPixmap ChangeImage::addPoint(QPixmap &map,QList<QPoint> point)
{
    QPixmap newmap(100,100);
    newmap = map;
    const int number = 300;
    int max = newmap.width()>newmap.height()?newmap.width():newmap.height();
    QPainter paint;
    int len =0;
    while(len<point.size()){
        if(paint.begin(&newmap)){
            paint.setBrush(Qt::red);
            paint.drawEllipse(point[len].x()-max/number/2,point[len].y()-max/number/2,max/number,max/number);
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
