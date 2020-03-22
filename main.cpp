#include "mainwindow.h"
#include "eventhandler.h"

#include <QApplication>
//#include "myapplication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MyApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.show();
    EventHandler hdlr;
    hdlr.connect(&hdlr,&EventHandler::scndChanged,&w,&MainWindow::nextSecond);
    hdlr.connect(&hdlr,&EventHandler::nearZeroAlert,&w,&MainWindow::showWindow);
    hdlr.connect(&w,&MainWindow::restNow,&hdlr,&EventHandler::setState);
    hdlr.connect(&w,&MainWindow::pause,&hdlr,&EventHandler::enableTimer);
    hdlr.setState(EventHandler::STATE_CTDN);
    return a.exec();
}
