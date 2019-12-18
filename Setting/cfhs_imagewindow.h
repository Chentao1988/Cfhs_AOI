#ifndef CFHS_IMAGEWINDOW_H
#define CFHS_IMAGEWINDOW_H
#include <QPushButton>
#include <QWidget>
#include <QPointer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QMap>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QStyleOption>
#include <QPainter>
#include <QFileDialog>
#include <QAction>
#include <QWidgetAction>
#include <QDebug>
#include <QLabel>
#include <QRect>
#include <QPainter>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>
#include <QButtonGroup>
class QAction;
class QMenu;

class Cfhs_ImageWindow : public QWidget
{
    Q_OBJECT
public:
    explicit Cfhs_ImageWindow(QWidget *parent = nullptr);
    ~Cfhs_ImageWindow();
    static QString getFunctionButtonStyle(const QString& name);

public slots:
    void setImage(const QString& path);
    void getAllPoint(QMap<int,QList<QPoint>> &map);
    void getAllPoint(QMap<int,QList<QList<QPoint>>> &map);
    void loadPoint(QMap<int,QList<QPoint>> getmap);
    void loadPoint(QMap<int,QList<QList<QPoint>>> getmap);
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void sig_sendRoiPoint(QMap<int,QList<QList<QPoint>>> &map);

private:
    QAction *getAction(const QString& imagePath, const QString& title);
    void setWindowStyle();
    void init();
    //绘画圆形
    void CdrawEllipse();
    void CdrawEllipse_map(int x,int y);
    void SaveEllipse();
    //绘画方形
    void CRect();
    void Crect_map(int x,int y);
    void SaveRect();
    //绘制线
    void CLineTo(int x=0,int y=0);
    void CLine_map(int x,int y);
    void SaveLine();
    void delectpoint(int x,int y);
    //保存函数，参数为保存地址以及文件名
    bool saveImage(QString savepath);
    QPushButton *getButton(const QString &name);

    QList<QPoint> list_thistoimg(QList<QPoint> listpoint);
    QList<QPoint> list_imgtothis(QList<QPoint> listpoint);
private slots:
    void onZoomInImage(void);
    void onZoomOutImage(void);
    void onPresetImage(void);

    void ClickButton_rect();
    void ClickButton_elli();
    void ClickButton_line();
    void ClickButton_save();
    void ClickButton_del();
    void ClickButton_zoomin();
    void ClickButton_zoomout();
    void ClickButton_adaptive();

private:
    QMap<int,QList<QPoint>> savemap_point;
    QList<QPoint> Rect_start_save_point;//保存绘制矩形的起始点
    QList<QPoint> Rect_end_save_point;  //保存绘制矩形的终点

    QList<QPoint> Ellipse_start_save_point;//保存绘制圆形的起始点
    QList<QPoint> Ellipse_end_save_point;  //保存绘制圆形的终点

    QList<QPoint> Line_save_point;
    QHBoxLayout* pLayout;
    QImage m_Image;
    qreal m_ZoomValue;
    int m_XPtInterval;  //偏移量X
    int m_YPtInterval;  //偏移量Y
    QPoint m_OldPos;
    int save_width;
    int save_height;
    QButtonGroup *pButtonGroup;
// 用于保存图片
    QPixmap save_map;
    QImage m_Image1;
    int select;
    QPoint point_start;
    QPoint p_start;
    QPoint p_end;
    QPoint point_show;
    QPoint center_Point;
    bool move_flg;

    QPushButton *button_zoomin;  //放大
    QPushButton *button_zoomout;  //缩小
    QPushButton *button_adaptive;  //自适应
    QPushButton *button_rect;  //矩形
    QPushButton *button_elli;  //圆形
    QPushButton *button_line;  //多边形
    QPushButton *button_save;  //保存
    QPushButton *button_del;  //删除

    bool m_Pressed;
    QPointer<QAction> m_zoomInAction;  //放大
    QPointer<QAction> m_zoomOutAction; //缩小
    QPointer<QAction> m_presetAction;  //还原
    QPointer<QMenu> m_menu;
};

#endif // CFHS_IMAGEWINDOW_H
