#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ozw-admin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nodes.cpp \
    util.cpp

HEADERS  += mainwindow.h \
    nodes.h \
    util.h

FORMS    += mainwindow.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libopenzwave
