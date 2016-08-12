#include "devicedbxmlreader.hpp"

#include <QtWidgets>


DeviceDBXMLReader::DeviceDBXMLReader(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
    QStyle *style = treeWidget->style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileIcon));
}

bool DeviceDBXMLReader::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == "ManufacturerSpecificData")
            readXBEL();
        else
            xml.raiseError(QObject::tr("The file is not an ManufacturerSpecific file."));
    }

    return !xml.error();
}

QString DeviceDBXMLReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}

void DeviceDBXMLReader::readXBEL()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "ManufacturerSpecificData");

    while (xml.readNextStartElement()) {
        if (xml.name() == "Manufacturer")
            readManufacturer(0);
        else
            xml.skipCurrentElement();
    }
}

void DeviceDBXMLReader::readProduct(QTreeWidgetItem *item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "Product");
    QTreeWidgetItem *product = createChildItem(item);
    //product->setFlags(item->flags() & Qt::ItemIsSelectable);
    product->setText(0, xml.attributes().value("name").toString());
}

void DeviceDBXMLReader::readSeparator(QTreeWidgetItem *item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "separator");

    QTreeWidgetItem *separator = createChildItem(item);
    separator->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    separator->setText(0, QString(30, 0xB7));
    xml.skipCurrentElement();
}

void DeviceDBXMLReader::readManufacturer(QTreeWidgetItem *item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "Manufacturer");

    QTreeWidgetItem *folder = createChildItem(item);
//    folder->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    folder->setText(0, xml.attributes().value("name").toString());
    while (xml.readNextStartElement()) {
        if (xml.name() == "Product")
            readProduct(folder);
        else
            qDebug() << "Skipping " << xml.name().toString();
            xml.skipCurrentElement();
    }
}

void DeviceDBXMLReader::readBookmark(QTreeWidgetItem *item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "bookmark");

    QTreeWidgetItem *bookmark = createChildItem(item);
    bookmark->setFlags(bookmark->flags() | Qt::ItemIsEditable);
    bookmark->setIcon(0, bookmarkIcon);
    bookmark->setText(0, QObject::tr("Unknown title"));
    bookmark->setText(1, xml.attributes().value("href").toString());

    while (xml.readNextStartElement()) {
#if 0
        if (xml.name() == "title")
            //readTitle(bookmark);
        else
            xml.skipCurrentElement();
#endif
    }
}

QTreeWidgetItem *DeviceDBXMLReader::createChildItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem *childItem;
    if (item) {
        childItem = new QTreeWidgetItem(item);
    } else {
        childItem = new QTreeWidgetItem(treeWidget);
    }
    childItem->setData(0, Qt::UserRole, xml.name().toString());
    return childItem;
}
