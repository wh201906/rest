#include "mysettings.h"

MySettings::MySettings(const QString &fileName, QSettings::Format format, QObject *parent)
    : QSettings(fileName, format, parent) {}

QVariant MySettings::value(const QString &key) const
{
    if(key == "isSpl")
        return QSettings::value(key, isSpl);
    else if(key == "Wh")
        return QSettings::value(key, Wh);
    else if(key == "Wm")
        return QSettings::value(key, Wm);
    else if(key == "Ws")
        return QSettings::value(key, Ws);
    else if(key == "Rh")
        return QSettings::value(key, Rh);
    else if(key == "Rm")
        return QSettings::value(key, Rm);
    else if(key == "Rs")
        return QSettings::value(key, Rs);
}
