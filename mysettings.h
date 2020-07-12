#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>
#include <QMap>
#include <QDebug>

class MySettings : public QSettings
{
    Q_OBJECT
public:
    typedef QMap<QString, QVariant> Items;

    MySettings(const QString &fileName, Format format, QObject *parent = nullptr);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    MySettings::Items getDefault();

    MySettings::Items getCurrent();

    void setValues(MySettings::Items items);
private:
    MySettings::Items defaultSettings;
};

#endif // MYSETTINGS_H
