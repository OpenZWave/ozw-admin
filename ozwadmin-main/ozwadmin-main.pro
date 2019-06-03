#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui xml remoteobjects websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../ozwadmin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    util.cpp \
    logwindow.cpp

HEADERS  += mainwindow.h \
    util.h \
    logwindow.h \

FORMS    += mainwindow.ui

LIBS += ../devicedb-lib/libdevicedb-lib.a ../ozwadmin-widgets/libozwadmin-widgets.a -L../../qt-openzwave/qt-openzwave/ -lqt-openzwave
INCLUDEPATH += ../devicedb-lib ../ozwadmin-widgets ../../qt-openzwave/qt-openzwave/include/

macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    #QMAKE_MAC_SDK = macosx10.11
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
}
