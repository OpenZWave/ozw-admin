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
    target.path = /usr/local/bin
    INSTALLS += target
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
    BUNDLE.files = $$OZW_LIB_PATH/libopenzwave-1.6.dylib $$QTOZW_LIB_PATH/libqt-openzwave.1.dylib $$QTOZW_LIB_PATH/../qt-openzwavedatabase/libqt-openzwavedatabase.1.dylib 
    BUNDLE.path = Contents/Frameworks/
    QMAKE_BUNDLE_DATA += BUNDLE
    MakeBundle.commands = $$[QT_HOST_BINS]/macdeployqt ../ozwadmin.app && $$top_srcdir/scripts/macdeployqtfix.py ../ozwadmin.app/Contents/MacOS/ozwadmin $$[QT_INSTALL_PREFIX]
    QMAKE_EXTRA_TARGETS += MakeBundle
    QMAKE_POST_LINK += $$MakeBundle.commands
    ICON = res/ozw_logo.icns
}

