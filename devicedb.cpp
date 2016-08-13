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
