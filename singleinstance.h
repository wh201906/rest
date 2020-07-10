#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QTimer>
#include <QThread>
#include <QSharedMemory>
#include <QDebug>

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
        NOT_READY,
        INITIALIZED,
        UNKNOWN_ERROR,
    };
    explicit SingleInstance(const QString &token, QObject *parent = nullptr);
    SingleInstance::RunningState getRunningState();
    SingleInstance::State getState();
    void wait(int msec);
    void init();
    void sendMessage();
private:
    QTimer* timer;
    State state = NOT_READY;
    RunningState runningState = UNKNOWN;
    int val;
    int interval = 1000;
signals:
    void newInstance();
private slots:
    void nextSecond();
};

#endif // SINGLEINSTANCE_H

// A dumb class used for single instance support.
//
// When the second app instance is created, the existing one will detect it and emit newInstance() signal.
//
// The value in the shared memory will be updated continuously by the existing app instance,
// which means that if the first app instance crashes, the second one can detect it and be created normally.
// (However, this situation doesn't happen on Windows, and I haven't test this feature on Linux yet.)
//
// Since the shared memory is only written by the existing instance, I didn't use lock() and unlock().
