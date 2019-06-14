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

#include <unistd.h>

#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QLoggingCategory>
#include <QDirIterator>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
#if 0
    QLoggingCategory::setFilterRules("qt.remoteobjects.debug=true\n"
                                     "qt.remoteobjects.warning=true\n"
                                     "qt.remoteobjects.models.debug=true\n"
                                     "qt.remoteobjects.models.debug=true\n"
                                     "qt.remoteobjects.io.debug=true\n"
                                     "default.debug=true");
#else
    QLoggingCategory::setFilterRules("default.debug=true");
#endif

    QCoreApplication::setOrganizationName("OpenZWave");
    QCoreApplication::setOrganizationDomain("openzwave.net");
    QCoreApplication::setApplicationName("ozw-admin");
    QApplication a(argc, argv);

    QDir configpath(".");
    QDirIterator it(":/config/", QStringList() << "*.xml" << "*.png" << "*.xsd", QDir::NoFilter, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo fi(it.next());
        QString dirname = fi.dir().path().remove(0, 2);
        configpath.mkpath(dirname);
        if (!QFileInfo::exists(configpath.path()+fi.filePath().remove(0,1))) {
            qDebug() << "Copying " << fi.filePath() << " to " << configpath.filePath(fi.filePath().remove(0, 2));
            QFile::copy(fi.filePath(), configpath.filePath(fi.filePath().remove(0, 2)));
        } else {
            qDebug() << "Skipping " << fi.filePath() << " as it exists";
        }
    }

    MainWindow w;
    w.show();
    return a.exec();
}
