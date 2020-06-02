#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui xml svg remoteobjects widgets

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
    value_delegate.cpp \
    qt-ads/ads_globals.cpp \
    qt-ads/DockAreaWidget.cpp \
    qt-ads/DockAreaTabBar.cpp \
    qt-ads/DockContainerWidget.cpp \
    qt-ads/DockManager.cpp \
    qt-ads/DockWidget.cpp \
    qt-ads/DockingStateReader.cpp \
    qt-ads/DockWidgetTab.cpp \
    qt-ads/FloatingDockContainer.cpp \
    qt-ads/FloatingDragPreview.cpp \
    qt-ads/DockOverlay.cpp \
    qt-ads/DockSplitter.cpp \
    qt-ads/DockAreaTitleBar.cpp \
    qt-ads/ElidingLabel.cpp \
    qt-ads/IconProvider.cpp \
    qt-ads/DockComponentsFactory.cpp


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
    value_delegate.h \
    qt-ads/ads_globals.h \
    qt-ads/DockAreaWidget.h \
    qt-ads/DockAreaTabBar.h \
    qt-ads/DockContainerWidget.h \
    qt-ads/DockManager.h \
    qt-ads/DockWidget.h \
    qt-ads/DockWidgetTab.h \ 
    qt-ads/DockingStateReader.h \
    qt-ads/FloatingDockContainer.h \
    qt-ads/FloatingDragPreview.h \
    qt-ads/DockOverlay.h \
    qt-ads/DockSplitter.h \
    qt-ads/DockAreaTitleBar.h \
    qt-ads/ElidingLabel.h \
    qt-ads/IconProvider.h \
    qt-ads/DockComponentsFactory.h 

unix {
    HEADERS += qt-ads/linux/FloatingWidgetTitleBar.h
    SOURCES += qt-ads/linux/FloatingWidgetTitleBar.cpp
}

FORMS    += HelpEditorDlg.ui \
	ListDialog.ui \
	bitsetwidget.ui \
	nodeflagswidget.ui

include(../ozw-admin.pri)

INCLUDEPATH += ../devicedb-lib ../ozwadmin-main qt-ads

DEFINES += ADS_STATIC

macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    #QMAKE_MAC_SDK = macosx10.11
#    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
}

RESOURCES += \
    propertybrowser/qtpropertybrowser.qrc \
    qt-ads/ads.qrc

#DISTFILES += \
#    propertybrowser/images/cursor-arrow.png \
#    propertybrowser/images/cursor-busy.png \
#    propertybrowser/images/cursor-closedhand.png \
#    propertybrowser/images/cursor-cross.png \
#    propertybrowser/images/cursor-forbidden.png \
#    propertybrowser/images/cursor-hand.png \
#    propertybrowser/images/cursor-hsplit.png \
#    propertybrowser/images/cursor-ibeam.png \
#    propertybrowser/images/cursor-openhand.png \
#    propertybrowser/images/cursor-sizeall.png \
#    propertybrowser/images/cursor-sizeb.png \
#    propertybrowser/images/cursor-sizef.png \
#    propertybrowser/images/cursor-sizeh.png \
#    propertybrowser/images/cursor-sizev.png \
#    propertybrowser/images/cursor-uparrow.png \
#    propertybrowser/images/cursor-vsplit.png \
#    propertybrowser/images/cursor-wait.png \
#    propertybrowser/images/cursor-whatsthis.png \
#    propertybrowser/qtpropertybrowser.pri
