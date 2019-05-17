#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

TEMPLATE = subdirs

CONFIG += ordered

#SUBDIRS = qt-openzwave simpleclient ozwadmin-widgets devicedb-lib ozwadmin-main

SUBDIRS = qt-openzwave simpleclient ozwadmin-main

ozwadmin-main.depends = qt-openzwave ozwadmin-widgets devicedb-lib
