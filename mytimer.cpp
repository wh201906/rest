#include "mytimer.h"

MyTimer::MyTimer(QObject *parent) : QTimer(parent)
{
    this->setTimerType(Qt::CoarseTimer);
    this->setInterval(1000);
    connect(this, &MyTimer::timeout, this, &MyTimer::nextSecond);
    state = STATE_IDLE;
    range.setRect(-3, -3, 6, 6);
}

MyTimer::~MyTimer()
{
}

void MyTimer::closeScreen()
{
    LockWorkStation();
}

void MyTimer::openScreen()
{

}

void MyTimer::nextSecond()
{

    if(state == STATE_CTDN)
    {
        currScnds--;
        if(currScnds <= 0)
            setState(STATE_REST);
        else if(currScnds == 30)
            emit nearZeroAlert();
        else if(currScnds == ctdnScnds - 1)
            emit newRound();
    }
    else if(state == STATE_REST)
    {
        QPoint move = QCursor().pos() - pos;
        if(currScnds > 0)
        {
            currScnds--;
            if(sigInterval == 0)
                closeScreen();
            sigInterval = (sigInterval + 1) % MAXSIGINTERVAL;
            if(!range.contains(move))
            {
                currScnds = restScnds;
                closeScreen();
            }
            pos = QCursor().pos();
        }
        else
        {
            if(!isScreenOpened)
            {
                openScreen();
                isScreenOpened = false;
            }
            if(!range.contains(move))
                setState(STATE_CTDN);
        }
    }
//    qDebug()<<"out:"<<currScnds;
    emit scndChanged(state, currScnds);
}



void MyTimer::setCtdnTime(int time)
{
    ctdnScnds = time;
}

void MyTimer::setRestTime(int time)
{
    restScnds = time;
}

void MyTimer::setState(timerState st)
{
    state = st;
    if(state == STATE_IDLE)
        this->stop();
    else
    {
        sigInterval = 0;
        if(state == STATE_CTDN)
            currScnds = ctdnScnds;
        else
        {
            currScnds = restScnds;
            isScreenOpened = false;
        }
        this->start();
    }
}

void MyTimer::enableTimer(bool st)
{
    if(st)
        this->start();
    else
        this->stop();
}
