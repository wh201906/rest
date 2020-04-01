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
    MyTimer hdlr(&a);
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.show();
    hdlr.connect(&hdlr, &MyTimer::scndChanged, &w, &MainWindow::nextSecond);
    hdlr.connect(&hdlr, &MyTimer::nearZeroAlert, &w, &MainWindow::showWindow);
    hdlr.connect(&hdlr, &MyTimer::newRound, &w, &MainWindow::hideWindow);
    hdlr.connect(&w, &MainWindow::restNow, &hdlr, &MyTimer::setState);
    hdlr.connect(&w, &MainWindow::pause, &hdlr, &MyTimer::enableTimer);
    hdlr.setState(MyTimer::STATE_CTDN);
    return a.exec();
}
