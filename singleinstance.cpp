#include "singleinstance.h"

#define valPtr static_cast<int*>(this->data())
#define newInsPtr (static_cast<int*>(this->data()) + 1)

SingleInstance::SingleInstance(const QString &token, QObject *parent): QSharedMemory(token, parent)
{
    state = IDLE;
    runningState = UNKNOWN;
    val = 0;
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setTimerType(Qt::VeryCoarseTimer);
    connect(timer, &QTimer::timeout, this, &SingleInstance::nextSecond);
    this->attach();
    if(this->error() == SingleInstance::NoError)
    {
        state = READ;
        val = *valPtr;
    }
    else if(this->error() == QSharedMemory::NotFound)
    {
        if(this->create(9))
        {
            runningState = NOT_RUNNING;
            state = WRITE;
            *valPtr = val;
            *newInsPtr = 0;
        }
        else
        {
            state = UNKNOWN_ERROR;
        }
    }
    else
    {
        state = UNKNOWN_ERROR;
    }
    if(state != UNKNOWN_ERROR)
        timer->start();
}

void SingleInstance::nextSecond()
{
    if(state == READ)
    {
        if(*valPtr != val)
        {
            runningState = RUNNING;
            timer->stop();
            *newInsPtr = 1;
        }
    }
    else if(state == WRITE)
    {
        val = (val + 1) % 10;
        *valPtr = val;
        if(*newInsPtr == 1)
        {
            *newInsPtr = 0;
            emit newInstance();
        }
    }
}

SingleInstance::State SingleInstance::getState()
{
    return state;
}

SingleInstance::RunningState SingleInstance::getRunningState()
{
    return runningState;
}
