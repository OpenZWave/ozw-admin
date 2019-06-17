#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ozwadmin-widgets
TEMPLATE = lib
CONFIG += staticlib silent


SOURCES += widgets.cpp \
    bitsetwidget.cpp \
    node_delegate.cpp \
    nodeflagswidget.cpp \
    propertybrowser/qtbuttonpropertybrowser.cpp \
    propertybrowser/qteditorfactory.cpp \
    propertybrowser/qtgroupboxpropertybrowser.cpp \
    propertybrowser/qtpropertybrowser.cpp \
    propertybrowser/qtpropertybrowserutils.cpp \
    propertybrowser/qtpropertymanager.cpp \
    propertybrowser/qttreepropertybrowser.cpp \
    propertybrowser/qtvariantproperty.cpp \
    value_delegate.cpp

HEADERS  += widgets.h \
    bitsetwidget.h \
    node_delegate.h \
    nodeflagswidget.h \
    propertybrowser/qtbuttonpropertybrowser.h \
    propertybrowser/qteditorfactory.h \
    propertybrowser/qtgroupboxpropertybrowser.h \
    propertybrowser/qtpropertybrowser.h \
    propertybrowser/qtpropertybrowserutils_p.h \
    propertybrowser/qtpropertymanager.h \
    propertybrowser/qttreepropertybrowser.h \
    propertybrowser/qtvariantproperty.h \
    value_delegate.h

FORMS    += HelpEditorDlg.ui \
	ListDialog.ui \
	bitsetwidget.ui \
	nodeflagswidget.ui

INCLUDEPATH += ../devicedb-lib ../ozwadmin-main ../../qt-openzwave/qt-openzwave/include/


macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    #QMAKE_MAC_SDK = macosx10.11
#    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11

}

RESOURCES += \
    propertybrowser/qtpropertybrowser.qrc

DISTFILES += \
    propertybrowser/images/cursor-arrow.png \
    propertybrowser/images/cursor-busy.png \
    propertybrowser/images/cursor-closedhand.png \
    propertybrowser/images/cursor-cross.png \
    propertybrowser/images/cursor-forbidden.png \
    propertybrowser/images/cursor-hand.png \
    propertybrowser/images/cursor-hsplit.png \
    propertybrowser/images/cursor-ibeam.png \
    propertybrowser/images/cursor-openhand.png \
    propertybrowser/images/cursor-sizeall.png \
    propertybrowser/images/cursor-sizeb.png \
    propertybrowser/images/cursor-sizef.png \
    propertybrowser/images/cursor-sizeh.png \
    propertybrowser/images/cursor-sizev.png \
    propertybrowser/images/cursor-uparrow.png \
    propertybrowser/images/cursor-vsplit.png \
    propertybrowser/images/cursor-wait.png \
    propertybrowser/images/cursor-whatsthis.png \
    propertybrowser/qtpropertybrowser.pri
