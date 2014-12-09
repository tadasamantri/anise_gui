#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T12:03:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = anise_GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    FileHandler.cpp

HEADERS  += mainwindow.h \
    FileHandler.h

FORMS    += mainwindow.ui


DESTDIR = "../build"

