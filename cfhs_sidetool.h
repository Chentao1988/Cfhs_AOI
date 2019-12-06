#ifndef CFHS_SIDETOOL_H
#define CFHS_SIDETOOL_H

#include <QWidget>
#include <QPointer>

class QPushButton;
class QFrame;



class Cfhs_SideTool : public QWidget
{
    Q_OBJECT

public:
    Cfhs_SideTool(QWidget *parent = nullptr);
    ~Cfhs_SideTool();
    void setSideEnable(const bool &isEnable);
    void setSideShow(const bool &isShow);
    bool isSideShow() const;
    static QString getDirFromFilePath(const QString &file);

private:
    QPushButton* addButton(const QString& title, const QString& iconName);

private slots:
    void on_offlineButton_clicked();
    void on_importImageButton_clicked();
    void on_exportImageButton_clicked();
    void on_retestButton_clicked();
    void on_prevImageButton_clicked();
    void on_nextImageButton_clicked();
    void on_cycleButton_clicked();
    void on_findIDButton_clicked();
    void on_loadFileAreaButton_clicked();
    void on_reportQueryButton_clicked();
    void on_showButton_clicked();

private:
    QPointer<QPushButton> m_offlineButton;
    QPointer<QPushButton> m_importImageButton;
    QPointer<QPushButton> m_exportImageButton;
    QPointer<QPushButton> m_retestButton;
    QPointer<QPushButton> m_prevImageButton;
    QPointer<QPushButton> m_nextImageButton;
    QPointer<QPushButton> m_cycleButton;
    QPointer<QPushButton> m_findIDButton;
    QPointer<QPushButton> m_loadFileAreaButton;
    QPointer<QPushButton> m_reportQueryButton;
    QPointer<QPushButton> m_showButton;
    QPointer<QFrame> m_buttonFrame;
    bool m_isShowed;  //窗口是否隐藏
    bool m_isOfflineOpened;  //离线是否开启
};

#endif // CFHS_SIDETOOL_H
