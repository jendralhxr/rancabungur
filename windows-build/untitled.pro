#-------------------------------------------------
#
# Project created by QtCreator 2012-02-01T09:42:26
#
# Heru Rosadi 0810630062
# Zulhaj Aliyansyah 0810633091
#
# waktu kita nyasar ke Lapan-Pusteksat Rancabungur
#
#-------------------------------------------------

include (C:/Qwt-6.0.1/features/qwt.prf)

INCLUDEPATH  += D:/PKL/qextserialport
QMAKE_LIBDIR += D:/PKL/qextserialport/build

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

CONFIG(debug, debug|release):LIBS  += -lqextserialport
else:LIBS  += -lqextserialport
win32:LIBS += -lsetupapi
