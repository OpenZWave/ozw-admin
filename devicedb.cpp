#include <QtWidgets>

#include "devicedb.hpp"
#include "devicedbxmlreader.hpp"
#include "ui_devicedb.h"

DeviceDB::DeviceDB(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceDB)
{
    ui->setupUi(this);

    /* laod the XML Files */
    ui->DeviceTreeList->clear();
    QFile mfxml("/Users/justinhammond/Development/open-zwave/config/manufacturer_specific.xml");
    if (!mfxml.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("QXmlStream Bookmarks"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(mfxml.fileName())
                                 .arg(mfxml.errorString()));
            return;
        }

        DeviceDBXMLReader reader(ui->DeviceTreeList);
        if (!reader.read(&mfxml)) {
            QMessageBox::warning(this, tr("QXmlStream Bookmarks"),
                                 tr("Parse error in file %1:\n\n%2")
                                 .arg(mfxml.fileName())
                                 .arg(reader.errorString()));
        } else {
            statusBar()->showMessage(tr("File loaded"), 2000);
        }

}

DeviceDB::~DeviceDB()
{
    delete ui;
}
