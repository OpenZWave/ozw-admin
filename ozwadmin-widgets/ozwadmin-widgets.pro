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
    bitsetwidget.cpp \
    value_delegate.cpp

HEADERS  += widgets.h \
    bitsetwidget.h \
    value_delegate.h

FORMS    += HelpEditorDlg.ui \
	ListDialog.ui \
	bitsetwidget.ui

INCLUDEPATH += ../devicedb-lib ../ozwadmin-main ../../qt-openzwave/qt-openzwave/include/


macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    #QMAKE_MAC_SDK = macosx10.11
#    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11

}
