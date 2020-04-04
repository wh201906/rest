#include "mainwindow.h"
#include "mytimer.h"
#include "singleinstance.h"

#include <QApplication>
//#include "myapplication.h"

int main(int argc, char *argv[])
{
    QString token = "10db51c0-d387-4834-8984-661cc53259a6";
     qDebug()<<token;
    QApplication a(argc, argv);
    // MyApplication a(argc, argv);
    MainWindow w;
    MyTimer mytimer(&a);
    SingleInstance snglIns(token);
//    qDebug()<<"start"<<QTime::currentTime();
    snglIns.init();
//    qDebug()<<"stop"<<QTime::currentTime();
    if(snglIns.getRunningState() == SingleInstance::NOT_RUNNING)
    {
        w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        w.setAttribute(Qt::WA_TranslucentBackground);
        w.show();
        mytimer.connect(&mytimer, &MyTimer::scndChanged, &w, &MainWindow::nextSecond);
        mytimer.connect(&mytimer, &MyTimer::nearZeroAlert, &w, &MainWindow::showWindow);
        mytimer.connect(&mytimer, &MyTimer::newRound, &w, &MainWindow::hideWindow);
        mytimer.connect(&w, &MainWindow::restNow, &mytimer, &MyTimer::setState);
        mytimer.connect(&w, &MainWindow::pause, &mytimer, &MyTimer::enableTimer);
        w.connect(&snglIns,&SingleInstance::newInstance,&w,&MainWindow::showWindow);
        mytimer.setState(MyTimer::STATE_CTDN);
//        // make a crash
//        QThread::sleep(1);
//        static_cast<MyTimer*>(nullptr)->nearZeroAlert();
        int rtnCode = a.exec();
        return rtnCode;
    }
    else
    {
        snglIns.sendMessage();
        qDebug()<<snglIns.getRunningState();
        return 0;
    }
}
