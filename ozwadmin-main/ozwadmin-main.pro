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
    deviceinfo.cpp \
    logwindow.cpp \
    mainwindow.cpp \
    metadatawindow.cpp \
    nodestatus.cpp \
    nodetablewidget.cpp \
    startup.cpp \
    startupprogress.cpp \
    statusbarmessages.cpp \
    util.cpp \
    valuetable.cpp

HEADERS  += mainwindow.h \
    configuration.h \
    deviceinfo.h \
    logwindow.h \
    metadatawindow.h \
    nodestatus.h \
    nodetablewidget.h \
    startup.h \
    startupprogress.h \
    statusbarmessages.h \
    util.h \ \
    valuetable.h

FORMS    += mainwindow.ui \
    configuration.ui \
    deviceinfo.ui \
    logwindow.ui \
    metadatawindow.ui \
    nodestatus.ui \
    nodetablewidget.ui \
    startup.ui \
    startupprogress.ui

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
        debug: LIBS += -L..\devicedb-lib\debug\ -L..\ozwadmin-widgets\debug\ -L..\..\qt-openzwave\qt-openzwave\debug\ -L..\..\qt-openzwave\qt-openzwavedatabase\debug\
    }
    CONFIG(release, debug|release) {
        release: LIBS += -L..\devicedb-lib\release\ -L..\ozwadmin-widgets\release\ -L..\..\qt-openzwave\qt-openzwave\release\ -L..\..\qt-openzwave\qt-openzwavedatabase\release\
    }
    LIBS += -ldevicedb-lib -lozwadmin-widgets -lqt-openzwave1 -lqt-openzwavedatabase1
    message($$LIBS)
    RC_ICONS=res/ozw_logo.ico
}

INCLUDEPATH += ../devicedb-lib ../ozwadmin-widgets


macx: {
    LIBS += -framework IOKit -framework CoreFoundation
    BUNDLE.files = $$OZW_LIB_PATH/libopenzwave-1.6.dylib $$QTOZW_LIB_PATH/libqt-openzwave.1.dylib $$QTOZW_LIB_PATH/../qt-openzwavedatabase/libqt-openzwavedatabase.1.dylib 
    BUNDLE.path = Contents/Frameworks/
    OZWDB.files = $$absolute_path($$QTOZW_LIB_PATH/../qt-openzwavedatabase/qt-openzwavedatabase.rcc)
    OZWDB.path = Contents/Resources/
    QMAKE_BUNDLE_DATA += BUNDLE
    QMAKE_BUNDLE_DATA += OZWDB
    MakeBundle.commands = $$[QT_HOST_BINS]/macdeployqt ../ozwadmin.app && $$top_srcdir/scripts/macdeployqtfix.py ../ozwadmin.app/Contents/MacOS/ozwadmin $$[QT_INSTALL_PREFIX]
    QMAKE_EXTRA_TARGETS += MakeBundle
    QMAKE_POST_LINK += $$MakeBundle.commands
    ICON = res/ozw_logo.icns
}

