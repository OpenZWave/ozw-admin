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
    ui(new Ui::DeviceDB),
    m_Ready(false),
    m_Path("config/")
{
    ui->setupUi(this);
    deviceTree = new DeviceDBXMLReader(this);
    deviceDetails = new DeviceConfigXMLReader(ui->DeviceDetails, this);

    while (!this->LoadXML()) {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory);
        if (dialog.exec()) {
            m_Path = dialog.directory().absolutePath();
        } else {
            break;
        }
    }
    deviceDetails->setPath(m_Path);
    connect(deviceTree, SIGNAL(setupManufacturerPage(QDomElement)), deviceDetails, SLOT(setupManufacturerPage(QDomElement)));
    connect(deviceTree, SIGNAL(setupProductPage(QDomElement)), deviceDetails, SLOT(setupProductPage(QDomElement)));
    this->ui->horizontalLayout->insertWidget(0, deviceTree);

}

bool DeviceDB::LoadXML() {
    QFile mfxml(m_Path+"/manufacturer_specific.xml");
    if (!mfxml.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("QXmlStream Bookmarks"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(mfxml.fileName())
                                 .arg(mfxml.errorString()));
            this->setReady(false);
            return false;
    }
    if (deviceTree->read(&mfxml)) {
        statusBar()->showMessage(tr("File loaded"), 2000);
        this->setReady(true);
        return true;
    } else {
        qWarning() << "Could Not Load Config Files";
        this->setReady(false);
        return false;
    }
}

void DeviceDB::doProductPage(QTreeWidgetItem *item) {

}


DeviceDB::~DeviceDB()
{
    delete ui;
}


bool DeviceDB::isReady() const
{
    return this->m_Ready;
}
void DeviceDB::setReady(bool ready) {
    this->m_Ready = ready;
}
