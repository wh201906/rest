#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>

class SingleInstance : public QObject
{
    Q_OBJECT
public:
    explicit SingleInstance(QObject *parent = nullptr);

signals:

};

#endif // SINGLEINSTANCE_H
