#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui widgets xml remoteobjects websockets svg
#CONFIG  += silent

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

include(../ozw-admin.pri)

unix {
    LIBS += -L../devicedb-lib/ -ldevicedb-lib -L../ozwadmin-widgets/ -lozwadmin-widgets
}
windows {
    CONFIG(debug, debug|release) {
        LIBS += -L..\devicedb-lib\debug\ -L..\ozwadmin-widgets\debug\ -L..\..\qt-openzwave\qt-openzwave\debug\
    } else {
        LIBS += -L..\devicedb-lib\release\ -L..\ozwadmin-widgets\release\ -L..\..\qt-openzwave\qt-openzwave\release\
    }
    LIBS += -ldevicedb-lib -lozwadmin-widgets -lqt-openzwave1
    message($$LIBS)
    message($$PWD)
}

INCLUDEPATH += ../devicedb-lib ../ozwadmin-widgets


macx: {
    LIBS += -framework IOKit -framework CoreFoundation
    BUNDLE.files = ../../qt-openzwave/qt-openzwave/libqt-openzwave.1.dylib ../../open-zwave/libopenzwave-1.6.dylib ../../qt-openzwave/qt-openzwavedatabase/libqt-openzwavedatabase.1.dylib
    BUNDLE.path = Contents/Frameworks/
    QMAKE_BUNDLE_DATA += BUNDLE
    ICON = res/ozw_logo.icns
}

