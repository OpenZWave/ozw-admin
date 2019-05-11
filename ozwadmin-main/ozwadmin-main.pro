#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui xml remoteobjects

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../ozwadmin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nodes.cpp \
    util.cpp \
    logwindow.cpp \
    associations.cpp \
    valueid.cpp \
    qtvalueidlists.cpp \
    valueiddelegate.cpp

HEADERS  += mainwindow.h \
    nodes.h \
    util.h \
    logwindow.h \
    associations.h \
    valueid.h \
    qtvalueidlists.h \
    valueiddelegate.h \

FORMS    += mainwindow.ui

LIBS += ../devicedb-lib/libdevicedb-lib.a ../ozwadmin-widgets/libozwadmin-widgets.a ../qt-openzwave/libqt-openzwave.a
INCLUDEPATH += ../devicedb-lib ../ozwadmin-widgets ../qt-openzwave


#for now, we will link against a static version of openzwave (Dev branch)
unix {
     LIBS += $$top_srcdir/open-zwave/libopenzwave.a -lresolv
     INCLUDEPATH += $$top_srcdir/open-zwave/cpp/src/
     libopenzwave.commands = cd $$top_srcdir/open-zwave && make -f Makefile
     QMAKE_EXTRA_TARGETS += libopenzwave
     PRE_TARGETDEPS += libopenzwave
}


unix:!macx  {
#    CONFIG += link_pkgconfig
#    PKGCONFIG += libopenzwave
     LIBS += -ludev
}
macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    #QMAKE_MAC_SDK = macosx10.11
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
}
