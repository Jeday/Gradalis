#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T20:24:23
#
#-------------------------------------------------

QT       += core gui\
            sql \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gradalis
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include(/home/je_day/Downloads/qt-solutions-master/qt-solutions-master/qtsoap/src/qtsoap.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    editclients.cpp \
    editchecks.cpp \
    debug_windget.cpp \
    edit_dialog.cpp \
    edit_items.cpp

HEADERS  += mainwindow.h \
    editclients.h \
    editchecks.h \
    main_head.h \
    debug_windget.h \
    edit_dialog.h \
    edit_items.h \
    models.h

#INCLUDEPATH += \
#    "C:\postgres32\include"

#LIBS += \
# "C:\postgres32\lib\libpq.lib"

DISTFILES +=

RESOURCES += \
    res.qrc

