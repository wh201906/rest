#ifndef REPORTWRITER_H
#define REPORTWRITER_H

#include <QObject>
#include <QMutex>
#include <QFile>
#include <QDebug>

class ReportWriter : public QObject
{
    Q_OBJECT
public:
    explicit ReportWriter(QObject *parent = nullptr);

    void setFileName(const QString &name);
    void setRecording(bool st);
public slots:
    void write(const QString &msg);
private:
    static QMutex writeMutex;
    QFile* file;
    QString fileName;
    bool isRecording = false;
};

#endif // REPORTWRITER_H
