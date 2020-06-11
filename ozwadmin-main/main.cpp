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
#include <QStyleFactory>

#include <qt-openzwave/qt-openzwavedatabase.h>

#include "util.h"
#include "mainwindow.h"

Q_LOGGING_CATEGORY(ozwadmin, "ozwadmin");
Q_LOGGING_CATEGORY(ozwvd, "ozwadmin.valuedelgate");


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
									 "ozwadmin.debug=true\n"
                                     "ozwadmin.*=true\n");
#else
    QLoggingCategory::setFilterRules("default.debug=true");
#endif
    QCoreApplication::setOrganizationName("OpenZWave");
    QCoreApplication::setOrganizationDomain("openzwave.net");
    QCoreApplication::setApplicationName("ozw-admin");
    QCoreApplication::setApplicationVersion(DEF2STR(APP_VERSION));
    QApplication a(argc, argv);
    QApplication::setOverrideCursor(Qt::WaitCursor);


    QCommandLineParser parser;
    parser.setApplicationDescription("OpenZWave Admin GUI");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption serialPort(QStringList() << "s" << "serial-port",
        "AutoStart connecting to the Specified Serial Port of the USB Stick",
        "serialPort"
    );
    parser.addOption(serialPort);

    QCommandLineOption remoteHost(QStringList() << "host",
        "AutoStart connecting to the Specified Remote Host/Port (1983 is default port)",
        "host:port"
    );
    parser.addOption(remoteHost);

    QCommandLineOption remoteKey(QStringList()  << "password",
        "Remote Host Password (optional)",
        "password"
    );
    parser.addOption(remoteKey);

    QCommandLineOption configDir(QStringList() << "c" << "config-dir",
        "Directory containing the OZW Config Files",
        "configDir"
    );

    parser.addOption(configDir);

    QCommandLineOption userDir(QStringList() << "u" << "user-dir",
        "Directory for the OZW User Files",
        "userDir"
    );

    parser.addOption(userDir);


    parser.process(a);

    QSettings settings;

    QString dbPath, userPath;

    if (parser.isSet(configDir)) {
        QStringList PossibleDBPaths;
        PossibleDBPaths << parser.value(configDir);
        QString path;
        foreach(path, PossibleDBPaths) {
            qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path)).absoluteFilePath() << " for manufacturer_specific.xml";
            if (QFileInfo(QDir::toNativeSeparators(path+"/manufacturer_specific.xml")).exists()) {
                dbPath = QFileInfo(QDir::toNativeSeparators(path).append("/")).absoluteFilePath();
                break;
            }
        }
        /* if we dont have a dbPath, Deploy our config files there */
        if (dbPath.isEmpty()) {
#ifndef Q_OS_WIN
            qCWarning(ozwadmin) << "Configuration Database Does Not Exist - Copying Database to Location " << QFileInfo(parser.value(configDir).append("/")).absoluteFilePath();
            QStringList Locations;
            Locations << ".";
            if (initConfigDatabase(Locations)) {
                copyConfigDatabase(QFileInfo(parser.value(configDir).append("/")).absoluteFilePath());
            } else {
                qCWarning(ozwadmin) << "Cant find qt-openzwavedatabase.rcc";
                QCoreApplication::exit(-1);
            }
            dbPath = QFileInfo(parser.value(configDir).append("/")).absoluteFilePath();
#else 
            qCWarning(ozwadmin) << "Configuration Database Not Found";
            QCoreApplication::exit(-1);
#endif
        }
    } else {
        /* search Default Locations for Config Files */
        QStringList PossibleDBPaths;
        PossibleDBPaths << settings.value("openzwave/ConfigPath", QDir::toNativeSeparators("../../../config/")).toString().append("/");
        PossibleDBPaths << "./config/";
        PossibleDBPaths << QDir::toNativeSeparators("../../../config/");
        PossibleDBPaths << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
        QString path;
        foreach(path, PossibleDBPaths) {
            qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path)).absoluteFilePath() << " for manufacturer_specific.xml";
            if (QFileInfo(QDir::toNativeSeparators(path+"/manufacturer_specific.xml")).exists()) {
                dbPath = QFileInfo(QDir::toNativeSeparators(path).append("/")).absoluteFilePath();
                break;
            }
        }
        /* if we dont have a dbPath, Deploy our config files there */
        if (dbPath.isEmpty()) {
#ifndef Q_OS_WIN
            dbPath = QFileInfo("./").absoluteFilePath();
            qCWarning(ozwadmin) << "Configuration Database Does Not Exist - Copying Database to Default Location " << dbPath;
            QStringList Locations;
            Locations << ".";
            if (initConfigDatabase(Locations)) {
                copyConfigDatabase(dbPath);
            } else {
                qCWarning(ozwadmin) << "Cant find qt-openzwavedatabase.rcc";
                QCoreApplication::exit(-1);
            }
#else 
            qCWarning(ozwadmin) << "Configuration Database Not Found";
            QCoreApplication::exit(-1);
#endif
        }
    }
    settings.setValue("openzwave/ConfigPath", dbPath);

    if (parser.isSet(userDir)) {
        QStringList PossibleCfgPaths;
        PossibleCfgPaths << parser.value(userDir);
        QString path;
        foreach(path, PossibleCfgPaths) {
            qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path)).absoluteFilePath() << " for options.xml";
            if (QFileInfo(QDir::toNativeSeparators(path+"/options.xml")).exists()) {
                userPath = QFileInfo(QDir::toNativeSeparators(path).append("/")).absoluteFilePath();
                break;
            }
        }
        /* if we dont have a userPath, Deploy our config files there */
        if (userPath.isEmpty()) {
#ifndef Q_OS_WIN
            qCWarning(ozwadmin) << "User Configuration Path Does Not Exist - Copying Config Files to Location " << QFileInfo(parser.value(userDir).append("/")).absoluteFilePath();
            QStringList Locations;
            Locations << ".";
            if (initConfigDatabase(Locations)) {
                copyUserDatabase(QFileInfo(parser.value(userDir).append("/")).absoluteFilePath());
            } else {
                qCWarning(ozwadmin) << "Cant find qt-openzwavedatabase.rcc";
                QCoreApplication::exit(-1);
            }
            userPath = QFileInfo(parser.value(userDir).append("/")).absoluteFilePath();
#else 
            qCWarning(ozwadmin) << "Configuration Database Not Found";
            QCoreApplication::exit(-1);
#endif
        }
    } else {
        /* search Default Locations for Config Files */
        QStringList PossibleUserPaths;
        PossibleUserPaths << settings.value("openzwave/UserPath", QDir::toNativeSeparators("../../../config/")).toString().append("/");
        PossibleUserPaths << "./config/";
        PossibleUserPaths << QDir::toNativeSeparators("../../../config/");
        PossibleUserPaths << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
        QString path;
        foreach(path, PossibleUserPaths) {
            qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path)).absoluteFilePath() << " for options.xml";
            if (QFileInfo(QDir::toNativeSeparators(path+"/options.xml")).exists()) {
                userPath = QFileInfo(QDir::toNativeSeparators(path).append("/")).absoluteFilePath();
                break;
            }
        }
        /* if we dont have a dbPath, Deploy our config files there */
        if (userPath.isEmpty()) {
#ifndef Q_OS_WIN
            userPath = QFileInfo("./").absoluteFilePath();
            qCWarning(ozwadmin) << "Configuration Database Does Not Exist - Copying Database to Default Location " << dbPath;
            QStringList Locations;
            Locations << ".";
            if (initConfigDatabase(Locations)) {
                copyUserDatabase(userPath);
            } else {
                qCWarning(ozwadmin) << "Cant find qt-openzwavedatabase.rcc";
                QCoreApplication::exit(-1);
            }
#else 
            qCWarning(ozwadmin) << "Configuration Database Not Found";
            QCoreApplication::exit(-1);
#endif
        }
    }
    settings.setValue("openzwave/UserPath", userPath);
#ifndef Q_OS_WIN
    finiConfigDatabase();
#endif
    qCInfo(ozwadmin) << "DBPath: " << dbPath;
    qCInfo(ozwadmin) << "userPath: " << userPath;


    MainWindow w;
    w.show();
    if (parser.isSet(serialPort)) {
        w.connectToLocal(parser.value(serialPort));
    } else if (parser.isSet(remoteHost)) {
        QString key;
        if (parser.isSet(remoteKey)) {
            key = parser.value(remoteKey);
        }
        QUrl server = QUrl::fromUserInput(parser.value(remoteHost), "");
        server.setScheme("ws");
        w.connectToRemote(server, key);
    }

    return a.exec();
}
