#include "reportwriter.h"

QMutex ReportWriter::writeMutex;

ReportWriter::ReportWriter(QObject *parent) : QObject(parent)
{
}

void ReportWriter::write(const QString& msg)
{
    if(!isRecording)
        return;
    qDebug() << "start writing";
    bool result;
    writeMutex.lock();
    file = new QFile(fileName);
    result = file->open(QFile::ReadWrite | QFile::Append | QFile::Text);
    if(!result)
    {
        qDebug() << "write failed";
        return;
    }
    file->write(msg.toLocal8Bit() + "\n");
    file->close();
    delete file;
    writeMutex.unlock();
}

void ReportWriter::setFileName(const QString& name)
{
    fileName = name;
    qDebug() << fileName;
}

void ReportWriter::setRecording(bool st)
{
    isRecording = st;
}
