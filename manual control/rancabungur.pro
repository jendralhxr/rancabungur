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

TARGET = rancabungur
TEMPLATE = app

SOURCES += main.cpp \
    serspanel.cpp \
    serscontrol.cpp

HEADERS += \
    serspanel.h \
    serscontrol.h

CONFIG(debug, debug|release):LIBS  += -lqextserialportd
else:LIBS  += -lqextserialport
win32:LIBS += -lsetupapi
