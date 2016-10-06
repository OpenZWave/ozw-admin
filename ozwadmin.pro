#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = ozwadmin-widgets devicedb-lib ozwadmin-main

ozwadmin-main.depends = ozwadmin-widgets devicedb-lib
