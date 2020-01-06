#include "changeimage.h"

ChangeImage::ChangeImage()
{

}

QPixmap ChangeImage::AddPoint_Draw(const QImage &img, QList<itoPoint> List_Point, const ShapeType &shape, bool pyr_flg)
{
    QPixmap re_img(100,100);
    if(img.isNull())
    {
        return QPixmap::fromImage(img);
    }
    re_img = QPixmap::fromImage(img);
    QPainter paint;
    int len =0;
    while(len<List_Point.size()){
        if(paint.begin(&re_img)){
            QPen pen(Qt::red);//定义画笔
            pen.setWidth(2);
            paint.setPen(pen);
            switch (shape) {
            case Line:
                paint.drawLine(List_Point[len].Start.x()/(static_cast<int>(pyr_flg+1)),List_Point[len].Start.y()/(static_cast<int>(pyr_flg+1))
                               ,List_Point[len].End.x()/(static_cast<int>(pyr_flg+1)),List_Point[len].End.y()/(static_cast<int>(pyr_flg+1)));
                break;
            case Rect:
                paint.drawRect(List_Point[len].Start.x()/(static_cast<int>(pyr_flg+1)),List_Point[len].Start.y()/(static_cast<int>(pyr_flg+1))
                               ,List_Point[len].End.x()/(static_cast<int>(pyr_flg+1)),List_Point[len].End.y()/(static_cast<int>(pyr_flg+1)));
                break;
            case Ellipse:
                paint.drawEllipse(List_Point[len].Start.x()/(static_cast<int>(pyr_flg+1)),List_Point[len].Start.y()/(static_cast<int>(pyr_flg+1))
                                  ,List_Point[len].End.x()/(static_cast<int>(pyr_flg+1)),List_Point[len].End.y()/(static_cast<int>(pyr_flg+1)));
                break;
            default:
                break;
            }
            paint.end();
        }
        else {
            qDebug()<<"Can not Painter";
            return re_img;
        }
        len++;
    }
    qDebug()<<"Add Point To Image Success!";
    return  re_img;
}


QPixmap ChangeImage::addPoint(const QPixmap &map,QList<QPoint> point,bool pyr_flg)
{

    QPixmap newmap(100,100);
    newmap = map.copy();
    if(newmap.isNull()){
        qDebug()<<"copy failed!";
        return map;
    }
    const int number = 300;
    int max = newmap.width()>newmap.height()?newmap.width():newmap.height();
    QPainter paint;
    int len =0;
    while(len<point.size()){
        if(paint.begin(&newmap)){
            paint.setBrush(Qt::red);
            paint.drawEllipse((point[len].x()/(static_cast<int>(pyr_flg+1))-max/number/2)
                              ,(point[len].y()/(static_cast<int>(pyr_flg+1))-max/number/2)
                              ,max/number,max/number);
            paint.end();
        }
        else {
            qDebug()<<"Can not Painter";
            return map;
        }
        len++;

    }
    qDebug()<<"Add Point Success!";
    return newmap;

}
