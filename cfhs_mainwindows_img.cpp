﻿#include "cfhs_mainwindows_img.h"



cfhs_mainwindows_img::cfhs_mainwindows_img(QWidget *parent)
    : QWidget(parent)
{
    this->init();
    this->setWindowStyle();
    setMouseTracking(true);
    painter=new QPainter;
    painter1=new QPainter;
    gridview_map = new QPixmap();
    result_map = new QPixmap();
    right_label_map = new QPixmap();
}

cfhs_mainwindows_img::~cfhs_mainwindows_img()
{

}

void cfhs_mainwindows_img::addPoint_begin()
{
//    painter->begin(gridview_map);
}
void cfhs_mainwindows_img::addPoint(QPoint point)
{
    save_list<<point;
#if 0
    QPainter painter3(gridview_map);
    painter3.setPen(Qt::red);//设置画笔颜色
    painter3.setBrush(Qt::red);
    painter3.drawEllipse(x-50,y-50,100,100);

    QPainter painter2(result_map);
    painter2.setPen(Qt::red);//设置画笔颜色
    painter2.setBrush(Qt::red);
    painter2.drawEllipse(x-50,y-50,100,100);
#else

//    painter->setPen(Qt::red);//设置画笔颜色
//    painter->setBrush(Qt::red);
//    painter->drawEllipse(point.x()-50,point.y()-50,100,100);

#endif
//    this->update();
}

void cfhs_mainwindows_img::addPoint_end()
{
    //result_map->toImage()得到image
    //QPixmap::fromImage(save_image) 得到pixmap
//    *result_map = *gridview_map;
//    painter->end();
}

void cfhs_mainwindows_img::AddLabel()
{

    QPainter painter_text(this);
    painter_text.setPen(Qt::green);//设置画笔颜色
    QFont font;
    font.setPointSize(16);
    painter_text.setFont(font);
    QString xpoint = QString("X : %1").arg(point_show.x());
    painter_text.drawText(10,30,xpoint);
    QString ypoint = QString("Y : %1 ").arg(point_show.y());
    painter_text.drawText(10,60,ypoint);
    QString gary_end = QString("R : %1").arg(Gray_show.red());
    painter_text.drawText(10,90,gary_end);
    QString gary_end1 = QString("G : %1").arg(Gray_show.green());
    painter_text.drawText(10,120,gary_end1);
    QString gary_end2 = QString("B : %1").arg(Gray_show.blue());
    painter_text.drawText(10,150,gary_end2);

}
//载入图片
void cfhs_mainwindows_img::setImage(const QString &path)
{
    QFile file(path);
    if (!file.exists()){
        return;
    }
    save_map.load(path);
    *right_label_map = save_map;
    *gridview_map = save_map;
    *result_map = save_map;
    save_list.clear();
    show_time.start();
    this->update();
}
//重载载入图片函数
void cfhs_mainwindows_img::setImage(const QImage img)
{
    save_map= QPixmap::fromImage(img);
    *right_label_map = save_map;
    *gridview_map = save_map;
    *result_map = save_map;
    save_list.clear();
    show_time.start();
    this->update();
}

void cfhs_mainwindows_img::setGridView(bool flg,int x,int y)
{
    gridview_flg = flg;
    set_x_gridview = x;
    set_y_gridview = y;
}

void cfhs_mainwindows_img::setPoint(QPoint p1,QPoint p2,QPoint p3,QPoint p4,bool flg_alphabet)
{
    p_left_top = p1;
    p_right_top = p2;
    p_left_bottom = p3;
    p_right_bottom = p4;
    alphabet_flg = flg_alphabet;    //是否方向判断
}


void cfhs_mainwindows_img::getImage(QPixmap& pixmap_img)
{
    int x_start,x_end,y_start,y_end;
    if(save_map.width() - save_map.height() < 0){
        x_start = (point_x-Up_Down_num*3*save_map.width()/min_lod);
        y_start = (point_y-Up_Down_num*3*save_map.height()/this->height());
        x_end = (Up_Down_num*3*save_map.width()/min_lod)*2;
        y_end = (Up_Down_num*3*save_map.height()/this->height())*2;
    }
    else {
        x_start = (point_x-Up_Down_num*3*save_map.width()/this->width());
        y_start = (point_y-Up_Down_num*3*save_map.height()/min_lod);
        x_end = (Up_Down_num*3*save_map.width()/this->width())*2;
        y_end = (Up_Down_num*3*save_map.height()/min_lod)*2;
    }
    if(x_start<0)
        x_start = save_point.x();
    if(y_start<0)
        y_start = save_point.y();
    if(x_start>save_map.width() - x_end)
        x_start = save_map.width() - x_end;
    if(y_start>save_map.height() - y_end)
        y_start = save_map.height() - y_end;
    save_point.setX(x_start);
    save_point.setY(y_start);
    pixmap_img=right_label_map->copy(x_start,y_start,x_end,y_end);
}


//保存函数
bool cfhs_mainwindows_img::saveImage(QString savepath)
{
    if(save_map.save(savepath)){
        return true;
    }
    return false;
}

void cfhs_mainwindows_img::getPoint(QPoint &left_top, QPoint &right_top,
                                    QPoint &left_bottom, QPoint &right_bottom, bool &flg_alphabet)
{
    left_top = p_left_top;
    right_top = p_right_top;
    left_bottom = p_left_bottom;
    right_bottom = p_right_bottom;
    flg_alphabet = alphabet_flg;
}

void cfhs_mainwindows_img::getGridView(bool &open_gridview, int &x_gridview, int &y_gridview)
{
    open_gridview = gridview_flg;
    x_gridview = set_x_gridview;
    y_gridview = set_y_gridview;
}

void cfhs_mainwindows_img::showLightPoint(const QPoint &point)
{
    p_light_point = point;
    this->update();
}

//鼠标右键
void cfhs_mainwindows_img::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();
    p_end = pos;
    this->update();
}

//  绘制函数

void cfhs_mainwindows_img::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(save_map.isNull())
        return;
    QPainter painter(this);
    if(save_map.width()-save_map.height()<0){
        min_lod = save_map.width()*this->height()/save_map.height();
        QRect picRect((this->width() - min_lod)/2, 0, min_lod, this->height());
        if(!gridview_flg)
            painter.drawPixmap(picRect, *result_map);
        else
            painter.drawPixmap(picRect, *gridview_map);
    }
    else {
        min_lod = save_map.height()*this->width()/save_map.width();
        QRect picRect(0,(this->height() - min_lod)/2, this->width(), min_lod);
        if(!gridview_flg)
            painter.drawPixmap(picRect, *result_map);
        else
            painter.drawPixmap(picRect, *gridview_map);
    }
    AddLabel(); //左上角信息
    CRect();    //绘制截取矩形
    CDrawPoint();// 加入缺陷坐标
    if(gridview_flg)//九宫格开启
        CDrawFourPoint();
}

void cfhs_mainwindows_img::CRect()
{
    QPainter pp(this);
    pp.setPen(Qt::green);//设置画笔颜色
    QRect temp(p_start,p_end);
    pp.drawRect(temp);//画矩形
}

void cfhs_mainwindows_img::CDrawPoint()
{
    if(save_list.size()<1)
        return;
    for (int i=0;i<save_list.size();i++) {
        QPoint save_point = save_list[i];
        QPoint point_da = imgtothis(save_point);
        QPainter pp(this);
        if(save_point == p_light_point)
        {
            //高亮显示绿色
            pp.setPen(Qt::green);
            pp.setBrush(Qt::green);
        }
        else
        {
            pp.setPen(Qt::red);//设置画笔颜色
            pp.setBrush(Qt::red);
        }

        pp.drawEllipse(point_da.x()-2,point_da.y()-2,4,4);
    }
}

void cfhs_mainwindows_img::CDrawFourPoint()
{
    QPoint A,B,C,D;
    A = imgtothis(p_left_top);
    B = imgtothis(p_right_top);
    C = imgtothis(p_left_bottom);
    D = imgtothis(p_right_bottom);
    int x,y;
    x = set_x_gridview;
    y = set_y_gridview;
    QPainter pp(this);
    pp.setPen(Qt::blue);//设置画笔颜色
    char alphabet[26];
    for (int i=0;i<26;i++){
        alphabet[i] = static_cast<char>('A'+i);
    }
    for (int i=0;i<=set_x_gridview;i++) {
        pp.drawLine((D.x() - A.x())*i/set_x_gridview + A.x(),A.y(),
                         (D.x() - A.x())*i/set_x_gridview + A.x(),D.y());
    }
    //横向x值差距
    for (int i=0;i<=set_y_gridview;i++) {
        pp.drawLine(A.x(),(D.y() - A.y())*i/set_y_gridview + A.y(),
                         D.x(),(D.y() - A.y())*i/set_y_gridview + A.y());
    }
    for (int i=1;i<=y;i++) {
        for (int j=1;j<=x;j++) {
            QString show_text;
            if(alphabet_flg){
                show_text = QString("%1%2").arg(alphabet[i-1]).arg(j);
            }
            else {
                show_text = QString("%1%2").arg(alphabet[y-i]).arg(x-j+1);
            }
            pp.drawText((D.x() - A.x())*j/x-20+A.x(),
                             (D.y() - A.y())*i/y-2+A.y(),show_text);
        }
    }
}

//鼠标按下
void cfhs_mainwindows_img::mousePressEvent(QMouseEvent *event)
{
    p_start = event->pos() ;   
}
//鼠标移动
void cfhs_mainwindows_img::mouseMoveEvent(QMouseEvent *event)
{
    if(save_map.isNull()){
        //qDebug()<<"save_map is null";
        return;
    }
    if(save_map.width() - save_map.height() < 0){
        point_x = event->pos().x()*save_map.width()/(min_lod)-(save_map.width()*((this->width() - min_lod)/2))/(min_lod);
        point_y = event->pos().y()*save_map.height()/(this->height());
    }
    else {
        point_y = event->pos().y()*save_map.height()/(min_lod) -(save_map.height()*(this->height() - min_lod)/2)/(min_lod);
        point_x = event->pos().x()*save_map.width()/(this->width());
    }
    if(point_x<0 || point_x>save_map.width() || point_y<0 || point_y>save_map.height()){
        point_x = 0;
        point_y = 0;
    }
    if(point_x>save_map.width()-1)
        point_x = save_map.width()-1;
    if(point_y>save_map.height()-1)
        point_y = save_map.height()-1;
    point_show.setX(point_x);   
    point_show.setY(point_y);
    QImage image_temp = save_map.toImage();
    Gray_show = image_temp.pixelColor(point_x,point_y);
    p_start.setX( event->pos().x()-Up_Down_num*3);
    p_start.setY( event->pos().y()-Up_Down_num*3);
    p_end.setX( event->pos().x()+Up_Down_num*3);
    p_end.setY( event->pos().y()+Up_Down_num*3);
    if(show_time.elapsed()>=10)
    {
        QPixmap sendmap;
        getImage(sendmap);
        emit sig_showImage(sendmap);
        show_time.start();
    }
    this->update();
}
//鼠标释放
void cfhs_mainwindows_img::mouseReleaseEvent(QMouseEvent *event)
{

    this->setCursor(Qt::ArrowCursor);
    QWidget::mouseReleaseEvent(event);
}

QAction *cfhs_mainwindows_img::getAction(const QString& imagePath, const QString& title)
{
    QAction *action = new QAction(this);
    QPixmap map;
    map.load(imagePath);
    map = map.scaled(32,32,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    QIcon icon;
    icon.addPixmap(map);
    action->setIcon(icon);
    action->setText(title);
    action->setIconText(title);
    return action;
}
//背景颜色设置
void cfhs_mainwindows_img::setWindowStyle()
{
    //界面背景色
    this->setStyleSheet("cfhs_mainwindows_img{background:#000A19}");
}
//初始化函数
void cfhs_mainwindows_img::init()
{
    this->setStyleSheet("background-color:lightGray");
    Up_Down_num = 5;
    set_x_gridview = 3;
    set_y_gridview = 3;
    gridview_flg = false;
    alphabet_flg = false;

    p_left_top = QPoint(0, 0);
    p_right_top = QPoint(0, 0);
    p_left_bottom = QPoint(0, 0);
    p_right_bottom = QPoint(0, 0);
    p_light_point = QPoint(0, 0);
}
//设置放大倍数
void cfhs_mainwindows_img::setTimes(int time_num)
{
    Up_Down_num = time_num;
}

QPoint cfhs_mainwindows_img::imgtothis(QPoint data)
{
    int x ;
    int y ;
    if(save_map.width() - save_map.height() < 0){
        x = (data.x()+(save_map.width()*(this->width() - min_lod)/2)/(min_lod))*min_lod/save_map.width();
        y = (data.y()*this->height()/save_map.height());
    }
    else {
        y = (data.y()+(save_map.height()*(this->height() - min_lod)/2)/(min_lod))*min_lod/save_map.height();
        x = (data.y()*this->width()/save_map.width());
    }
    QPoint return_point(x,y);
    return return_point;
}

QPoint cfhs_mainwindows_img::thistoimg(QPoint data)
{
    int x,y;
    if(save_map.width() - save_map.height() < 0){
        x = data.x()*save_map.width()/(min_lod)-(save_map.width()*((this->width() - min_lod)/2))/(min_lod);
        y = data.y()*save_map.height()/(this->height());
    }
    else {
        x = data.y()*save_map.height()/(min_lod) -(save_map.height()*(this->height() - min_lod)/2)/(min_lod);
        y = data.x()*save_map.width()/(this->width());
    }
    QPoint return_point(x,y);
    return return_point;
}