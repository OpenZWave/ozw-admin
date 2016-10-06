#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = ozwadmin-widgets devicedb-lib ozwadmin-main open-zwave

ozwadmin-main.depends = ozwadmin-widgets devicedb-lib open-zwave


#for now, we will link against a static version of openzwave (Dev branch)
unix {
     libopenzwave.commands = cd open-zwave && make -f Makefile
     QMAKE_EXTRA_TARGETS += libopenzwave
     PRE_TARGETDEPS += libopenzwave
    ozwadmin-main.depends += libopenzwave
}
