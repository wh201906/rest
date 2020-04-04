#include "singleinstance.h"

#define valPtr static_cast<int*>(this->data())
#define newInsPtr (static_cast<int*>(this->data()) + 1)

SingleInstance::SingleInstance(const QString &token, QObject *parent): QSharedMemory(token, parent)
{
    state = NOT_READY;
    runningState = UNKNOWN;
    val = 0;
    timer = new QTimer(this);
    timer->setTimerType(Qt::VeryCoarseTimer);
    connect(timer, &QTimer::timeout, this, &SingleInstance::nextSecond);
}

void SingleInstance::init()
{
    qDebug()<<this->attach();
    qDebug()<<this->error();
    if(this->error() == SingleInstance::NoError)
    {

        val = *valPtr;
//        qDebug()<<"start waiting..."<<QTime::currentTime();
        QThread::msleep(1.5*interval);
//        qDebug()<<"stop waiting..."<<QTime::currentTime();
        if(*valPtr!=val)
        {
            runningState=RUNNING;
        }
        else
        {
            runningState=NOT_RUNNING;
        }
        state = INITIALIZED;

    }
    else if(runningState==NOT_RUNNING || this->error() == QSharedMemory::NotFound)
    {
        if(this->create(9))
        {
            runningState = NOT_RUNNING;
            state = INITIALIZED;
            *valPtr = val;
            *newInsPtr = 0;
            timer->setInterval(interval);
            timer->start();
        }
    }
    if(state == NOT_READY)
        state = UNKNOWN_ERROR;
}

void SingleInstance::nextSecond()
{
    val = (val + 1) % 10;
    *valPtr = val;
    if(*newInsPtr == 1)
    {
        *newInsPtr = 0;
        emit newInstance();
    }
    qDebug()<<*valPtr<<","<<*newInsPtr;
}

SingleInstance::State SingleInstance::getState()
{
    return state;
}

SingleInstance::RunningState SingleInstance::getRunningState()
{
    return runningState;
}
void SingleInstance::sendMessage()
{
    *newInsPtr=1;
}
