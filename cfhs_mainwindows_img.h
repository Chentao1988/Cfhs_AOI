#ifndef CFHS_MAINWINDOWS_IMG_H
#define CFHS_MAINWINDOWS_IMG_H
#include <QLabel>
#include <QWidget>
#include <QPointer>
#include <QHBoxLayout>
#include <QMenu>
#include <QContextMenuEvent>
#include <QStyleOption>
#include <QPainter>
#include <QFileDialog>
#include <QAction>
#include <QWidgetAction>
#include <QDebug>
#include <QRect>
#include <QTime>
#include <QList>
#include <QPushButton>
#include <QButtonGroup>
#include "changeimage.h"
#pragma execution_character_set("utf-8")
class QAction;
class QMenu;



class cfhs_mainwindows_img : public QWidget
{
    Q_OBJECT
public:
    explicit cfhs_mainwindows_img(QWidget *parent = nullptr);
    ~cfhs_mainwindows_img();

public slots:
    //载入图片
    void setImage(const QString& path);
    void setImage(const QImage img);
    void setImage(const QImage img,QList<itoPoint>list_point, const ShapeType &shape = Line);

    //清除旧坐标点
    void ClearPoint();
    //放大倍数
    void setTimes(int num);
    //添加缺陷点
    void addPoint(QPoint point);

    //手动输入四个点以及是否反向判断
    void setPoint(QPoint left_top,QPoint right_bottom,bool flg_alphabet = true);
    //九宫格开启判断以及行列数
    void setGridView(bool open_gridview,int x_gridview = 3,int y_gridview = 3);
    //保存图片
    bool saveImage(QString savepath);

    void getPoint(QPoint &left_top,QPoint &right_bottom,
                  bool &flg_alphabet);
    //九宫格开启判断以及行列数
    void getGridView(bool &open_gridview,int &x_gridview,int &y_gridview);
    //高亮坐标点
    void showLightPoint(const QPoint &point);

    void setEnable(bool enable_flg);

    static QString getFunctionButtonStyle(const QString& name);
    QPushButton *getButton(const QString &name);
    void Clicked_DownLoad();


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QAction *getAction(const QString& imagePath, const QString& title);
    void setWindowStyle();
    void init();
    void AddLabel();
    void CRect();
    void CDrawPoint();
    void CDrawFourPoint();
    void getImage(QPixmap& pixmap_img);
    QPoint imgtothis(QPoint data);
    QPoint thistoimg(QPoint data);



private:
    QTime time;
    QPushButton *DownLoad_Button;
    QHBoxLayout* pLayout;
    QButtonGroup *pButtonGroup;
    QList<QPoint> save_list;
    QColor Gray_show;   //左上角显示的RGB颜色
    bool gridview_flg;  //九宫格开启flag
    bool alphabet_flg;  //反向图片flag
    int set_x_gridview; //九宫格行数
    int set_y_gridview; //九宫格列数
    int Up_Down_num;    //选择放大倍数
    int min_lod;        //背景显示出来的长度
    int point_x;        //鼠标当前在图片的位置x
    int point_y;        //鼠标当前在图片的位置y
    QPixmap save_map;   //初始载入图片的map
    QPixmap *right_label_map;//加入缺陷坐标的map
    QPixmap *gridview_map;  //加入九宫格的map
    QPixmap *result_map;
    QPixmap save_img;
    QPoint p_start;     //矩形开始点
    QPoint p_end;       //矩形结束点
    QPoint point_show;  //用于左上角显示的坐标
    QPoint p_left_top;  //A
    QPoint p_right_bottom;//D
    QTime show_time;    //发送img的时间
    QPoint save_point;  //防止矩形范围超出图片的记录点
    QPainter *painter;
    QPainter *painter1;
    QPoint p_light_point;  //高亮坐标点
signals:
    void sig_showImage(const QPixmap& pixmap);

};


#endif // CFHS_MAINWINDOWS_IMG_H
