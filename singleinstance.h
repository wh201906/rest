#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>
#include <QTimer>
#include <QSharedMemory>

class SingleInstance : public QObject
{
    Q_OBJECT
public:
    explicit SingleInstance(const QString& token);

private:
    QTimer* timer;
signals:

};

#endif // SINGLEINSTANCE_H
