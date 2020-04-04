#include "mainwindow.h"
#include "mytimer.h"

#include <QApplication>
//#include "myapplication.h"

int main(int argc, char *argv[])
{
    const QString token = QCoreApplication::applicationName() + QCoreApplication::applicationVersion();
    QApplication a(argc, argv);
    // MyApplication a(argc, argv);
    MainWindow w;
    MyTimer mytimer(&a);
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.show();
    mytimer.connect(&mytimer, &MyTimer::scndChanged, &w, &MainWindow::nextSecond);
    mytimer.connect(&mytimer, &MyTimer::nearZeroAlert, &w, &MainWindow::showWindow);
    mytimer.connect(&mytimer, &MyTimer::newRound, &w, &MainWindow::hideWindow);
    mytimer.connect(&w, &MainWindow::restNow, &mytimer, &MyTimer::setState);
    mytimer.connect(&w, &MainWindow::pause, &mytimer, &MyTimer::enableTimer);
    mytimer.setState(MyTimer::STATE_CTDN);
    return a.exec();
}
