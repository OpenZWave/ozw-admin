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
#include <QtWidgets>

#include "devicedb.hpp"

#include "ui_devicedb.h"

DeviceDB::DeviceDB(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceDB)
{
    ui->setupUi(this);

    QFile mfxml("config/manufacturer_specific.xml");
    if (!mfxml.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("QXmlStream Bookmarks"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(mfxml.fileName())
                                 .arg(mfxml.errorString()));
            return;
        }

        deviceTree = new DeviceDBXMLReader(this);
        deviceDetails = new DeviceConfigXMLReader(ui->DeviceDetails, this);
        connect(deviceTree, SIGNAL(setupManufacturerPage(QDomElement)), deviceDetails, SLOT(setupManufacturerPage(QDomElement)));
        connect(deviceTree, SIGNAL(setupProductPage(QDomElement)), deviceDetails, SLOT(setupProductPage(QDomElement)));
        //connect(ui->DeviceTreeList, SIGNAL(itemSelectionChanged()), reader, SLOT(itemSelectionChanged()));
        if (deviceTree->read(&mfxml)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
        }
        this->ui->horizontalLayout->insertWidget(0, deviceTree);

}

void DeviceDB::doProductPage(QTreeWidgetItem *item) {

}


DeviceDB::~DeviceDB()
{
    delete ui;
}
