#include "mytimer.h"

MyTimer::MyTimer(QObject *parent) : QTimer(parent)
{
    this->setTimerType(Qt::CoarseTimer);
    this->setInterval(1000);
    connect(this, &MyTimer::timeout, this, &MyTimer::nextSecond);
    state = STATE_IDLE;
}

MyTimer::~MyTimer()
{
}

void MyTimer::Lock()
{
    LockWorkStation();
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
        currScnds--;
        if(currScnds <= 0)
        {
            currScnds = 0;
            if(!lockState)
                setState(STATE_CTDN);
        }
        else if(currScnds == restScnds - 1)
            Lock();
        else if(isForceLock)
        {
            // 3 seconds for quiting the app forcefully when in rest mode.
            if(lockState)
                forceLockCounter = 0;
            else
            {
                forceLockCounter++;
                if(forceLockCounter >= 3)
                {
                    Lock();
                    forceLockCounter %= 3;
                }
            }
        }
        else if(!isForceLock)
        {
            if(!lockState)
                setState(STATE_CTDN);
        }
    }
    emit scndChanged(state, currScnds);
}

void MyTimer::setCtdnTime(int time)
{
    if(ctdnScnds != time)
    {
        ctdnScnds = time;
        if(state == STATE_CTDN)
        {
            currScnds = ctdnScnds;
        }
    }
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
        if(state == STATE_CTDN)
            currScnds = ctdnScnds;
        else
        {
            currScnds = restScnds;
        }
        this->start();
    }
}

MyTimer::timerState MyTimer::getState()
{
    return state;
}

void MyTimer::enableTimer(bool st)
{
    if(st)
        this->start();
    else
        this->stop();
}

void MyTimer::onLockStateChanged(bool state)
{
    lockState = state;
}

void MyTimer::setForceLock(bool state)
{
    isForceLock = state;
}
