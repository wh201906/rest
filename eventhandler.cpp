#include "eventhandler.h"

EventHandler::EventHandler(QObject *parent) : QObject(parent)
{
    qTimer=new QTimer(this);
    qTimer->setTimerType(Qt::CoarseTimer);
    qTimer->setInterval(1000);
    connect(qTimer,&QTimer::timeout,this,&EventHandler::nextSecond);
    state=STATE_IDLE;
    range.setRect(-3,-3,6,6);
}

EventHandler::~EventHandler()
{
    delete qTimer;
}

void EventHandler::closeScreen()
{
    PostMessageA(HWND_BROADCAST,WM_SYSCOMMAND, SC_MONITORPOWER, 2); //When using SendMessage, the application might crash
}

void EventHandler::openScreen()
{
    PostMessageA(HWND_BROADCAST,WM_SYSCOMMAND, SC_MONITORPOWER, -1); //When using SendMessage, the application might crash
}

void EventHandler::nextSecond()
{
    if(state==STATE_CTDN)
    {
        currScnds--;
        if(currScnds<=0)
            setState(STATE_REST);
        else if(currScnds==30)
            emit nearZeroAlert();
    }
    else if(state==STATE_REST)
    {
        QPoint move=QCursor().pos()-pos;
        if(currScnds>0)
        {
            currScnds--;
            closeScreen();
            if(!range.contains(move))
            {
                currScnds=restScnds;
            }
            pos=QCursor().pos();
        }
        else
        {
            openScreen();
            if(!range.contains(move))
                setState(STATE_CTDN);
        }
    }
    emit scndChanged(state,currScnds);
}



void EventHandler::setCtdnTime(int time)
{
    ctdnScnds=time;
}

void EventHandler::setRestTime(int time)
{
    restScnds=time;
}

void EventHandler::setState(timerState st)
{
    state=st;
    if(state==STATE_IDLE)
        qTimer->stop();
    else
    {
        if(state==STATE_CTDN)
            currScnds=ctdnScnds;
        else
            currScnds=restScnds;
        qTimer->start();
    }
}

void EventHandler::enableTimer(bool st)
{
    if(st)
        qTimer->start();
    else
        qTimer->stop();
}
