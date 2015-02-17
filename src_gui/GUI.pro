#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T12:03:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = anise_GUI
TEMPLATE = app
DESTDIR = "../build"

SOURCES += main.cpp\
        mainwindow.cpp \
    node.cpp \
    connection.cpp \
    mesh.cpp \
    gate.cpp \
    jsonfilehandler.cpp \
    settingshandler.cpp \
    anisecommunicator.cpp \
    nodefactory.cpp \
    nodecatalog.cpp \
    dragwidget.cpp \
    drawobject.cpp \
    mesheditorwidget.cpp \
    data.cpp \
    singletonrender.cpp \
    testdrawobject.cpp \
    nodetypelabel.cpp \
    parameterwindow.cpp \
    gatebutton.cpp


HEADERS  += mainwindow.h \
    node.h \
    connection.h \
    mesh.h \
    gate.h \
    jsonfilehandler.h \
    settingshandler.h \
    anisecommunicator.h \
    qdebugstreamredirector.h \
    nodefactory.h \
    nodecatalog.h \
    dragwidget.h \
    drawobject.h \
    mesheditorwidget.h \
    data.h \
    singletonrender.h \
    testdrawobject.h \
    nodetypelabel.h \
    parameterwindow.h \
    gatebutton.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++14

QMAKE_POST_LINK = cp -R ../Data $${DESTDIR}

