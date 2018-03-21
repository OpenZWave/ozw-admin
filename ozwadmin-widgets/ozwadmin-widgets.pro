#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ozwadmin-widgets
TEMPLATE = lib
CONFIG += staticlib


SOURCES += widgets.cpp \
    bitsetwidget.cpp

HEADERS  += widgets.h \
    bitsetwidget.h

FORMS    += HelpEditorDlg.ui \
	ListDialog.ui

INCLUDEPATH += ../devicedb-lib ../ozwadmin-main

unix {
     INCLUDEPATH += $$top_srcdir/open-zwave/cpp/src/
}

macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    #QMAKE_MAC_SDK = macosx10.11
}
