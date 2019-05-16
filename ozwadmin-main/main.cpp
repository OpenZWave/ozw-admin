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

#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QLoggingCategory>
#include <QTreeView>
#include "valueid.h"
#include "qtopenzwave.h"
#include "qtozwmanager.h"
#include "websocketiodevice.h"

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

    qRegisterMetaType<QtValueID>();
    qRegisterMetaType<uint8>("uint8");
    qRegisterMetaType<QVector<int>>("QVector<int>");
    QCoreApplication::setOrganizationName("OpenZWave");
    QCoreApplication::setOrganizationDomain("openzwave.net");
    QCoreApplication::setApplicationName("ozw-admin");
    QApplication a(argc, argv);

    QTOpenZwave *ozw = new QTOpenZwave();
    QTOZWManager *manager = ozw->GetManager();
    qDebug() << manager;

    QTreeView view;
    view.setWindowTitle(QStringLiteral("LocalView"));
    view.resize(640,480);
    view.setModel(manager->getNodeModel());
    view.show();

    QTreeView view2;
    view2.setWindowTitle(QStringLiteral("LocalView"));
    view2.resize(640,480);
    view2.setModel(manager->getValueModel());
    view2.show();

    QTreeView view3;
    view3.setWindowTitle(QStringLiteral("Associations"));
    view3.resize(640,480);
    view3.setModel(manager->getAssociationModel());
    view3.show();


    a.exec();
#if 0
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
#endif
}
