#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = qt-openzwave qt-openzwaveremote simpleclient ozwadmin-widgets devicedb-lib ozwadmin-main

qt-openzwave.dir = qt-openzwave
qt-openzwave.file = qt-openzwave/qt-openzwave.pro
qt-openzwaveremote.dir = qt-openzwave
qt-openzwaveremote.file = qt-openzwave/qt-openzwaveremote.pro

ozwadmin-main.depends = qt-openzwave ozwadmin-widgets devicedb-lib
