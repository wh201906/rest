QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mysettings.cpp \
    mytimer.cpp \
    settingdialog.cpp \
    singleinstance.cpp

HEADERS += \
    mainwindow.h \
    mysettings.h \
    mytimer.h \
    settingdialog.h \
    singleinstance.h

FORMS += \
    mainwindow.ui \
    settingdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

VERSION = 0.2.1
QMAKE_TARGET_PRODUCT = "Rest"
QMAKE_TARGET_DESCRIPTION = "Rest Reminder"
QMAKE_TARGET_COMPANY = "wh201906"

#RC_FILE = rest_resource.rc

win32: LIBS += -lwtsapi32
