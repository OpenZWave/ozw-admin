#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui xml

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
    devicedbxmlreader.cpp \
    deviceconfigxmlreader.cpp \
    widgets.cpp

HEADERS  += mainwindow.h \
    nodes.h \
    util.h \
    logwindow.h \
    associations.h \
    devicedb.hpp \
    devicedbxmlreader.hpp \
    deviceconfigxmlreader.h \
    widgets.h

FORMS    += mainwindow.ui \
    devicedb.ui \
    HelpEditorDlg.ui \
    ListDialog.ui

#for now, we will link against a static version of openzwave (Dev branch)
unix {
     LIBS += open-zwave/libopenzwave.a -lresolv -ludev
     INCLUDEPATH += open-zwave/cpp/src/

     libopenzwave.commands = cd open-zwave && make -f Makefile
     QMAKE_EXTRA_TARGETS += libopenzwave
     PRE_TARGETDEPS += libopenzwave
}


unix:!macx  {
#    CONFIG += link_pkgconfig
#    PKGCONFIG += libopenzwave
}
macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    QMAKE_MAC_SDK = macosx10.11
}
