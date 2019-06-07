#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = devicedb-lib
TEMPLATE = lib
CONFIG += staticlib



SOURCES += deviceconfigxmlreader.cpp \
	devicedb.cpp \
	devicedbxmlreader.cpp \
	devicequirks.cpp \
	commandclasslist.cpp \
    domitem.cpp \
    dommodel.cpp \
    widgetmapper.cpp

HEADERS  += deviceconfigxmlreader.h \
	devicequirks.h \
	devicedb.hpp \
	devicedbxmlreader.hpp \
	commandclasslist.h \
    domitem.h \
    dommodel.h \
    widgetmapper.h

FORMS    += devicedb.ui

INCLUDEPATH += ../ozwadmin-widgets/


macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    #QMAKE_MAC_SDK = macosx10.11
#    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11

}
