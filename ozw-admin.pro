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
    util.cpp \
    logwindow.cpp \
    associations.cpp \
    devicedb.cpp \
    devicedbxmlreader.cpp

HEADERS  += mainwindow.h \
    nodes.h \
    util.h \
    logwindow.h \
    associations.h \
    devicedb.hpp \
    devicedbxmlreader.hpp

FORMS    += mainwindow.ui \
    devicedb.ui

unix:!macx  {
    CONFIG += link_pkgconfig
    PKGCONFIG += libopenzwave
}
macx: {
    CONFIG += c++11
    INCLUDEPATH += $$PWD/../open-zwave/cpp/src/
    LIBS += -L$$PWD/../open-zwave -lopenzwave
    LIBS += $$PWD/../open-zwave/libopenzwave.a -framework IOKit -framework CoreFoundation
    QMAKE_MAC_SDK = macosx10.11
}
