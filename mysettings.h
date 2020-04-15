#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>

class MySettings : public QSettings
{
    Q_OBJECT
public:
//    explicit MySettings(const QString &organization,
//                        const QString &application = QString(), QObject *parent = nullptr);
//    MySettings(Scope scope, const QString &organization,
//               const QString &application = QString(), QObject *parent = nullptr);
//    MySettings(Format format, Scope scope, const QString &organization,
//               const QString &application = QString(), QObject *parent = nullptr);
    MySettings(const QString &fileName, Format format, QObject *parent = nullptr);
//    explicit MySettings(QObject *parent = nullptr);
//    explicit MySettings(Scope scope, QObject *parent = nullptr);
    QVariant value(const QString &key) const;
private:
    bool isSpl=false;
    int Wh=0;
    int Wm=40;
    int Ws=0;
    int Rh=0;
    int Rm=3;
    int Rs=0;
};

#endif // MYSETTINGS_H
