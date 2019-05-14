#-------------------------------------------------
#
# Project created by QtCreator 2019-05-10T15:07:50
#
#-------------------------------------------------

QT       -= gui
QT       += remoteobjects
QT       += websockets


TEMPLATE = lib

CONFIG += staticlib local


DEFINES += QTOPENZWAVE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        qtopenzwave.cpp \
    qtozwmanager.cpp \
    qtozwnotification.cpp \
    websocketiodevice.cpp \
    qtozwnodemodel.cpp \
    qtozwvalueidmodel.cpp

HEADERS += \
        qtopenzwave.h \
        qt-openzwave_global.h \  \
        websocketiodevice.h \
        qtozwmanager.h \
        qtozwnotification.h \
        qtozwnodemodel.h \
        qtozwvalueidmodel.h


INCLUDEPATH += ../open-zwave/cpp/src/

REPC_SOURCE =  qtozwmanager.rep
REPC_REPLICA =  qtozwmanager.rep

unix {
    target.path = /usr/lib
    INSTALLS += target
}

remote {
  TARGET = qt-openzwave_remote
  DEFINES += BUILD_REP
}

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
}
