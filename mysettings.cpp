#include "mysettings.h"

//MySettings::MySettings(const QString &organization, const QString &application, QObject *parent)
//    : QSettings(organization, application, parent) {}

//MySettings::MySettings(QSettings::Scope scope, const QString &organization, const QString &application, QObject *parent)
//    : QSettings(scope, organization, application, parent) {}

//MySettings::MySettings(QSettings::Format format, QSettings::Scope scope, const QString &organization, const QString &application, QObject *parent)
//    : QSettings(format, scope, organization, application, parent) {}

MySettings::MySettings(const QString &fileName, QSettings::Format format, QObject *parent)
    : QSettings(fileName, format, parent) {}

//MySettings::MySettings(QObject *parent)
//    : QSettings(parent) {}

//MySettings::MySettings(QSettings::Scope scope, QObject *parent)
//    : QSettings(scope, parent) {}

QVariant MySettings::value(const QString &key) const
{
    if(key=="isSpl")
        return QSettings::value(key,isSpl);
    else if(key=="Wh")
        return QSettings::value(key,Wh);
    else if(key=="Wm")
        return QSettings::value(key,Wm);
    else if(key=="Ws")
        return QSettings::value(key,Ws);
    else if(key=="Rh")
        return QSettings::value(key,Rh);
    else if(key=="Rm")
        return QSettings::value(key,Rm);
    else if(key=="Rs")
        return QSettings::value(key,Rs);
}
