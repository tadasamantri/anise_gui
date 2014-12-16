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
    node.cpp \
    connection.cpp \
    mesh.cpp \
    gate.cpp \
    jsonfilehandler.cpp \
    anisecommunicator.cpp \
    nodefactory.cpp

HEADERS  += mainwindow.h \
    node.h \
    connection.h \
    mesh.h \
    gate.h \
    jsonfilehandler.h \
    qdebugstreamredirector.h \
    anisecommunicator.h \
    nodefactory.h

FORMS    += mainwindow.ui


DESTDIR = "../build"

