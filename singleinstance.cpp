#include "singleinstance.h"

SingleInstance::SingleInstance(const QString &token)
{
    timer=new QTimer();
    timer->setInterval(1000);
    timer->setTimerType(Qt::VeryCoarseTimer);
}
