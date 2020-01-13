#include "cfhs_mainwindow.h"
#include "cfhs_logindialog.h"
#include <QApplication>
#include <QFile>
#include <QSplashScreen>
#include <QPixmap>
#include <QMessageBox>
#include <QFont>
#include <QImage>
#include <QMutex>


QMutex mutex;//日志代码互斥锁

//日志生成
void LogMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {
     mutex.lock();
     //Critical Resource of Code
     QByteArray localMsg = msg.toLocal8Bit();
     QString log;

     switch (type) {
     case QtDebugMsg:
     //将debug信息的输出位置保存到字符串中
     //file:文件所在的路径；function:所在的函数;line:所在的行；context:内容
         log.append(QString("Debug  File:%1 %2  Line:%3      Content:%4").arg(context.file).arg(context.function).arg(context.line).arg(msg));

         break;
     case QtInfoMsg:
         //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Info: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtWarningMsg:
         //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Warning: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtCriticalMsg:
         //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Critical: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtFatalMsg:
         //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Fatal: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         abort();
     }

     QFile file;
     QString path = QString("./log.txt");
     file.setFileName(path);
     if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
     {
         QString erinfo = file.errorString();
         qDebug()<<erinfo;
         return;
     }
     QTextStream out(&file);
     out << "\n\r" << log;
     file.close();

     mutex.unlock();
 }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //qInstallMessageHandler(LogMsgOutput);
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
    if(login.DialogShow() == QDialog::Accepted)
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
