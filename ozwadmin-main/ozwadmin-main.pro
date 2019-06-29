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

ozwconfig.target=config/qrc_ozwconfig.cpp
ozwconfig.commands=cp -R ../../open-zwave/config config/ && cd config && $$[QT_INSTALL_BINS]/rcc -project -o ozwconfig.qrc && $$[QT_INSTALL_BINS]/rcc --name="ozwconfig" --root="/config/" ozwconfig.qrc -o qrc_ozwconfig.cpp

QMAKE_EXTRA_TARGETS += ozwconfig
PRE_TARGETDEPS += config/qrc_ozwconfig.cpp
SOURCES += config/qrc_ozwconfig.cpp

INCLUDEPATH += ../../qt-openzwave/qt-openzwave/include/
LIBS += -L../../qt-openzwave/qt-openzwave/ -lqt-openzwave

macx: {
    LIBS += -framework IOKit -framework CoreFoundation
    BUNDLE.files = ../../qt-openzwave/qt-openzwave/libqt-openzwave.1.dylib ../../open-zwave/libopenzwave-1.6.dylib
    BUNDLE.path = Contents/Frameworks/
    QMAKE_BUNDLE_DATA += BUNDLE
    ICON = res/ozw_logo.icns
}

