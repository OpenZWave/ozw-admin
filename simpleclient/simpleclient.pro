QT += widgets remoteobjects websockets

SOURCES = main.cpp \
    mainwindow.cpp \
    startup.cpp

CONFIG   -= app_bundle
DEFINES  += remote

LIBS += ../qt-openzwave/libqt-openzwave.a ../open-zwave/libopenzwave.a -lresolv
INCLUDEPATH += ../qt-openzwave ../open-zwave/cpp/src

REPC_REPLICA = ../qt-openzwave/qtozwmanager.rep

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/remoteobjects/modelviewclient
#INSTALLS += target

requires(qtConfig(treeview))

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
}

FORMS += \
    mainwindow.ui \
    startup.ui

HEADERS += \
    mainwindow.h \
    startup.h
