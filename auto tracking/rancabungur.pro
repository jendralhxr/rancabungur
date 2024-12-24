#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T09:42:26
#
#-------------------------------------------------

include (/usr/local/qwt/features/qwt.prf)

INCLUDEPATH  += /me/qextserialport/src/
QMAKE_LIBDIR += /me/qextserialport/src//build

QT       += core gui

CONFIG += qwt

TARGET = rancaauto
TEMPLATE = app

SOURCES += main.cpp \
    serspanel.cpp \
    serscontrol.cpp \
    sersschedule.cpp \
    serslogger.cpp

HEADERS += \
    serspanel.h \
    serscontrol.h \
    sersschedule.h \
    serslogger.h

CONFIG(debug, debug|release):LIBS  += -lqextserialportd
else:LIBS  += -lqextserialport
win32:LIBS += -lsetupapi
