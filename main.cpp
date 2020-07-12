#include "mainwindow.h"
#include "mytimer.h"
#include "singleinstance.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QString token = "10db51c0-d387-4834-8984-661cc53259a6";
    qDebug() << token;
    QApplication a(argc, argv);
    MainWindow w;
    SingleInstance snglIns(token);
    snglIns.init();
    if(snglIns.getRunningState() == SingleInstance::NOT_RUNNING)
    {
        w.show();

        w.connect(&snglIns, &SingleInstance::newInstance, &w, &MainWindow::showWindow);
//        // make a crash
//        QThread::sleep(1);
//        static_cast<MyTimer*>(nullptr)->nearZeroAlert();
        int rtnCode = a.exec();
        return rtnCode;
    }
    else
    {
        snglIns.sendMessage();
        qDebug() << snglIns.getRunningState();
        return 0;
    }
}
