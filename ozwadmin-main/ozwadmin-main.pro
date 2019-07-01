#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui widgets xml remoteobjects websockets svg
CONFIG  += silent

TARGET = ../ozwadmin
TEMPLATE = app

SOURCES += main.cpp\
    configuration.cpp \
    logwindow.cpp \
    mainwindow.cpp \
    metadatawindow.cpp \
    util.cpp

HEADERS  += mainwindow.h \
    configuration.h \
    logwindow.h \
    metadatawindow.h \
    util.h \

FORMS    += mainwindow.ui \
    configuration.ui \
    logwindow.ui \
    metadatawindow.ui

RESOURCES += \
    ozwadmin-main.qrc \



LIBS += ../devicedb-lib/libdevicedb-lib.a ../ozwadmin-widgets/libozwadmin-widgets.a
INCLUDEPATH += ../devicedb-lib ../ozwadmin-widgets

INCLUDEPATH += ../../qt-openzwave/qt-openzwave/include/ ../../qt-openzwave/qt-openzwavedatabase/include/
LIBS += -L../../qt-openzwave/qt-openzwave/ -lqt-openzwave -L../../qt-openzwave/qt-openzwavedatabase -lqt-openzwavedatabase

macx: {
    LIBS += -framework IOKit -framework CoreFoundation
    BUNDLE.files = ../../qt-openzwave/qt-openzwave/libqt-openzwave.1.dylib ../../open-zwave/libopenzwave-1.6.dylib ../../qt-openzwave/qt-openzwavedatabase/libqt-openzwavedatabase.1.dylib
    BUNDLE.path = Contents/Frameworks/
    QMAKE_BUNDLE_DATA += BUNDLE
    ICON = res/ozw_logo.icns
}

