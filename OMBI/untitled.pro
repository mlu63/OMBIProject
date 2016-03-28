#-------------------------------------------------
#
# Project created by QtCreator 2013-10-24T15:51:14
#
#-------------------------------------------------
# fixdate
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    UltraData.cpp \
    Data.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    UltraData.h \
    Data.h

FORMS    += mainwindow.ui

QT += printsupport

RESOURCES += \
    Resources.qrc
