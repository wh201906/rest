#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>

class MySettings : public QSettings
{
    Q_OBJECT
public:
    MySettings(const QString &fileName, Format format, QObject *parent = nullptr);
    QVariant value(const QString &key) const;
private:
    bool isSpl = false;
    int Wh = 0;
    int Wm = 40;
    int Ws = 0;
    int Rh = 0;
    int Rm = 3;
    int Rs = 0;
};

#endif // MYSETTINGS_H
