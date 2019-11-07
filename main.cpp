#include "cfhs_mainwindow.h"
#include "cfhs_logindialog.h"
#include <QApplication>
#include <QFile>
#include <QSplashScreen>
#include <QPixmap>
#include <QMessageBox>
#include <QFont>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //加载qss
    QFile qss(":/style.qss");
    if(qss.open(QFile::ReadOnly))
    {
        QString style = qss.readAll();
        a.setStyleSheet(style);
        qss.close();
    }
    QString strInfo;  //日志
    //设置开机动画
    QPixmap map;
    map.load(":/main_background.png");
    QSplashScreen splash;
    QFont font;
    font.setPixelSize(36);
    font.setBold(true);
    font.setFamily("Mircosfot Yahei");
    splash.setFont(font);
    splash.setPixmap(map);
    splash.show();
    //启动数据库
    splash.showMessage("Loading database, please waiting ...", Qt::AlignCenter, QColor("#0077FF"));
    if(!m_logicInterface->Init(strInfo))
    {
        QMessageBox::warning(nullptr, " ", strInfo);
        return 0;
    }
    //显示登录界面
    splash.hide();
    Cfhs_LoginDialog login;
    if(login.exec() == QDialog::Accepted)
    {
        //加载主界面
        splash.showMessage("Loading module, please waiting ...", Qt::AlignCenter, QColor("#0077FF"));
        splash.show();
        Cfhs_MainWindow w;
        UserType user = login.getUser();
        w.setUser(user);
        splash.hide();
        w.show();

        return a.exec();
    }
    else
    {
        //关闭数据库
        m_logicInterface->Exit();
        return 0;
    }
}
