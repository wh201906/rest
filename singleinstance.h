#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QTimer>
#include <QSharedMemory>

class SingleInstance : public QSharedMemory
{
    Q_OBJECT
public:
    enum RunningState
    {
        RUNNING,
        NOT_RUNNING,
        UNKNOWN,
    };
    enum State
    {
        IDLE,
        READ,
        WRITE,
        UNKNOWN_ERROR,
    };
    explicit SingleInstance(const QString &token, QObject *parent = nullptr);
    SingleInstance::RunningState getRunningState();
    SingleInstance::State getState();
private:
    QTimer* timer;
    State state = IDLE;
    RunningState runningState=UNKNOWN;
    int val=0;
signals:
    void newInstance();
private slots:
    void nextSecond();
};

#endif // SINGLEINSTANCE_H
