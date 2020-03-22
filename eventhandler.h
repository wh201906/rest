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
    static void closeScreen();
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
    void scndChanged(timerState st, int currScnds);
    void nearZeroAlert();
private slots:
    void nextSecond();
private:
    QTimer* qTimer;
    timerState state=STATE_IDLE;
    int currScnds=0;
//    int ctdnScnds=40*60;
//    int restScnds=3*60;
    int ctdnScnds=20;
    int restScnds=10;
    QPoint pos;
    QRect range;
};

#endif // EVENTHANDLER_H
