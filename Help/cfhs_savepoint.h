#pragma once

#include <QtWidgets/QMainWindow>
#include <opencv2/opencv.hpp>
#include <string>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>
#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#pragma execution_character_set("utf-8")

typedef struct _NPoint
{
	int Shape;	//0为圆形，1为距形
    QPointF Start_Point;
    QPointF End_Point;
}NPoint;

class Cfhs_SavePoint : public QWidget
{
	Q_OBJECT

public:
    Cfhs_SavePoint(QWidget *parent = Q_NULLPTR);
    ~Cfhs_SavePoint();
	void LoadPath(QString path,QList<NPoint> &npoint);

public slots:
	QList<NPoint> GetList();

private slots:
	void onZoomInImage(void);
	void onZoomOutImage(void);
	void onPresetImage(void);
	void ClickButton_rect();
	void ClickButton_elli();
	void ClickButton_line();
	void ClickButton_move();
	void ClickButton_del();
private:
//	cv::Mat Img_Mat;
	QPoint p_Start;
	QPoint p_End;
	QList<NPoint>nPoint;
	QImage Img_Qimg;
	QPixmap save_map;

	QPushButton *button_zoomin;  //放大
	QPushButton *button_zoomout;  //缩小
	QPushButton *button_adaptive;  //自适应
	QPushButton *button_rect;  //矩形
	QPushButton *button_elli;  //圆形
//	QPushButton *button_line;  //多边形
	QPushButton *button_move;  //移动
	QPushButton *button_del;  //删除
	QPoint m_OldPos;
	QButtonGroup *pButtonGroup;
	QHBoxLayout* pLayout;

	int save_width;
	int save_height;
    float sub_height;
    float sub_width;

    double m_ZoomValue;
	int m_XPtInterval;
	int m_YPtInterval;
	bool m_Pressed;
	int select;

	void Init();
	void setWindowStyle();
	//绘画圆形
	void CdrawEllipse();
	void SaveEllipse();
	//绘画方形
	void CRect();
	void SaveRect();
	void Show_map(int x, int y);
	void delectpoint(int x, int y);

	static QString getFunctionButtonStyle(const QString& name);
	QPushButton *getButton(const QString &name);
    QPointF list_thistoimg(QPointF listpoint);
    QPointF list_imgtothis(QPointF listpoint);
	QList<NPoint> list_thistoimg(QList<NPoint> listpoint);
	QList<NPoint> list_imgtothis(QList<NPoint> listpoint);
	QImage Mat2QImage(cv::Mat& image);
	cv::Mat QImage2Mat(const QImage &image, bool inCloneImageData = true);


protected:
	void paintEvent(QPaintEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

};
