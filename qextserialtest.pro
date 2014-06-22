#-------------------------------------------------
#
# Project created by QtCreator 2014-06-22T20:51:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qextserialtest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        serial.cpp

HEADERS  += mainwindow.h\
            serial.h

FORMS    += mainwindow.ui

unix|win32: LIBS += -L$$PWD/lib -lQt5ExtSerialPort1

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
