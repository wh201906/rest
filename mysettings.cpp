#include "mysettings.h"

MySettings::MySettings(const QString &fileName, QSettings::Format format, QObject *parent)
    : QSettings(fileName, format, parent)
{
    defaultSettings.insert("isSimple", false);
    defaultSettings.insert("isForceLock", true);
    defaultSettings.insert("hasTaskbarItem", true);
    defaultSettings.insert("Wh", 0);
    defaultSettings.insert("Wm", 40);
    defaultSettings.insert("Ws", 0);
    defaultSettings.insert("Rh", 0);
    defaultSettings.insert("Rm", 3);
    defaultSettings.insert("Rs", 0);
    defaultSettings.insert("lastPositionX", 0);
    defaultSettings.insert("lastPositionY", 0);
}

QVariant MySettings::value(const QString &key, const QVariant &defaultValue) const
{
    qDebug() << key;
    if(defaultValue.isValid())
        return QSettings::value(key, defaultValue);
    else
        return QSettings::value(key, defaultSettings[key]);
}

MySettings::Items MySettings::getDefault()
{
    return defaultSettings;
}
MySettings::Items MySettings::getCurrent()
{
    MySettings::Items currItems = defaultSettings;
    QStringList keys = defaultSettings.keys(); // get all available values.
    for(int i = 0; i < keys.size(); i++)
    {
        qDebug() << MySettings::value(keys[i]);
        currItems[keys[i]] = MySettings::value(keys[i]);
    }
    return currItems;
}

void MySettings::setValues(MySettings::Items items)
{
    QStringList keys = items.keys(); // only set the values existing in the QMap.
    for(int i = 0; i < keys.size(); i++)
    {
        QSettings::setValue(keys[i], items[keys[i]]);
    }
}

// To add a new setting item:
// 1. add the new key and default value for the new item(when reading settings, the app will search all keys in defaultSttings)
// 2. put related codes in the MainWindow
// 3. make a proper UI to set this item in SettingDialog
