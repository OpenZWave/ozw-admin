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

#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QLoggingCategory>
#include "valueid.h"

int main(int argc, char *argv[])
{

    QLoggingCategory::setFilterRules("default.debug=true");

    qRegisterMetaType<QtValueID>();
    qRegisterMetaType<uint8>("uint8");
    qRegisterMetaType<QVector<int>>("QVector<int>");
    QCoreApplication::setOrganizationName("OpenZWave");
    QCoreApplication::setOrganizationDomain("openzwave.net");
    QCoreApplication::setApplicationName("ozw-admin");
    QApplication a(argc, argv);
    try {

        MainWindow w;
        w.show();
        return a.exec();
    } catch (OpenZWave::OZWException &e) {
        QMessageBox::critical(nullptr, "Exception",
                              QString("A unhandled Exception was caught: ").append(e.GetMsg().c_str()),
                              QMessageBox::Abort);
        exit(-1);
    }
}
