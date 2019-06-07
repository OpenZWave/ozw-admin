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
    metadatawindow.cpp \
    util.cpp \
    logwindow.cpp

HEADERS  += mainwindow.h \
    metadatawindow.h \
    util.h \
    logwindow.h \

FORMS    += mainwindow.ui \
    metadatawindow.ui

LIBS += ../devicedb-lib/libdevicedb-lib.a ../ozwadmin-widgets/libozwadmin-widgets.a
INCLUDEPATH += ../devicedb-lib ../ozwadmin-widgets

macx: {
    QMAKE_CXXFLAGS += -F../../qt-openzwave/qt-openzwave/
    LIBS += -framework IOKit -framework CoreFoundation
    LIBS += -F../../qt-openzwave/qt-openzwave/ -framework qt-openzwave
    BUNDLE.files = ../../qt-openzwave/qt-openzwave/qt-openzwave.framework/
    BUNDLE.path = Contents/Frameworks/qt-openzwave.framework/
    LIBOZW.files = ../../open-zwave/libopenzwave-1.6.dylib
    LIBOZW.path = Contents/Frameworks/
    QMAKE_BUNDLE_DATA += BUNDLE LIBOZW
    ICON = res/ozw_logo.icns
} else {
    LIBS += -L../../qt-openzwave/qt-openzwave/ -lqt-openzwave
    INCLUDEPATH += ../../qt-openzwave/qt-openzwave/include/
}

RESOURCES += \
    ozwadmin-main.qrc
