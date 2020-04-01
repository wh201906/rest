#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>
#include <windows.h>
#include <QTimer>
#include <QCursor>
#include <QRect>


class EventHandler : public QObject
{
    Q_OBJECT
public:
    explicit EventHandler(QObject *parent = nullptr);
    ~EventHandler();

    static void closeScreen(); // using Windows API to open/close screen
    static void openScreen();

    enum timerState
    {
        STATE_IDLE,
        STATE_CTDN,
        STATE_REST,
    };
    void setCtdnTime(int time);
    void setRestTime(int time);

public slots:
    void setState(timerState st);
    void enableTimer(bool st);
signals:
    void scndChanged(timerState st, int currScnds);// to refresh the timer on MainWindow
    void nearZeroAlert(); // to make MainWindow visible
private slots:
    void nextSecond();
private:
    QTimer* qTimer;
    timerState state=STATE_IDLE;

    int currScnds=0;
    int ctdnScnds=40*60; // work for 40min
    int restScnds=3*60; // rest for 3min
//    int ctdnScnds=20;
//    int restScnds=10;  // for debugging

    QPoint pos; // last mouse position
    QRect range; // maximum range for mouse to move(in case a slight move resets the timer)
};

#endif // EVENTHANDLER_H
