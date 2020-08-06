#ifndef MYTIMER_H
#define MYTIMER_H

#include <QObject>
#include <windows.h>
#include <QTimer>
#include <QCursor>
#include <QRect>
#include <QDebug>

class MyTimer : public QTimer
{
    Q_OBJECT
public:
    explicit MyTimer(QObject *parent = nullptr);
    ~MyTimer();

    static void Lock(); // using Windows API to lock the current session

    enum timerState
    {
        STATE_IDLE,
        STATE_CTDN,
        STATE_REST,
    };
    void setCtdnTime(int time);
    void setRestTime(int time);
    timerState getState();

    void setForceLock(bool state);
public slots:
    void setState(timerState st);
    void enableTimer(bool st);
    void onLockStateChanged(bool state);
signals:
    void scndChanged(timerState st, int currScnds);// to refresh the timer on MainWindow
    void nearZeroAlert(); // to make MainWindow visible
    void newRound(); // to make MainWindow invisible
private slots:
    void nextSecond();
private:
    timerState state = STATE_IDLE;
    bool lockState = false;
    bool isForceLock = true;
    int forceLockCounter = 0;

    int currScnds = 0;
    int ctdnScnds = 40 * 60; // work for 40min
    int restScnds = 3 * 60; // rest for 3min
};

#endif // MYTIMER_H
