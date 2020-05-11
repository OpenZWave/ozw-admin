/*  ozw-admin - Gui for OpenZWave
 *    Copyright (C) 2016  Justin Hammond <justin@dynam.ac>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _WIN32
#include <unistd.h>
#endif

#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QLoggingCategory>

#include "util.h"
#include "mainwindow.h"

Q_LOGGING_CATEGORY(ozwadmin, "ozwadmin");


#define DEF2STR2(x) #x
#define DEF2STR(x) DEF2STR2(x)


int main(int argc, char *argv[])
{


#if 1
    QLoggingCategory::setFilterRules("*.debug=false\n"
                                     "qt.remoteobjects.debug=false\n"
                                     "qt.remoteobjects.warning=true\n"
                                     "ozw.*.debug=true\n"
                                     "ozw.library.debug=false\n"
									 "ozwadmin.debug=true\n");
#else
    QLoggingCategory::setFilterRules("default.debug=true");
#endif

    QCoreApplication::setOrganizationName("OpenZWave");
    QCoreApplication::setOrganizationDomain("openzwave.net");
    QCoreApplication::setApplicationName("ozw-admin");
    QCoreApplication::setApplicationVersion(DEF2STR(APP_VERSION));
    QApplication a(argc, argv);


    MainWindow w;
    w.show();
    return a.exec();
}
