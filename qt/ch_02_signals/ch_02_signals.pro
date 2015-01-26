#-------------------------------------------------
#
# Project created by QtCreator 2015-01-21T16:37:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ch_02_signals
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    signalslotcounter.cpp \
    externaldialog.cpp

HEADERS  += mainwindow.h \
    signalslotcounter.h \
    externaldialog.h

FORMS    += mainwindow.ui \
    externaldialog.ui
