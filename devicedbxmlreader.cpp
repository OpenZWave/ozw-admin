#include "devicedbxmlreader.hpp"

#include <QtWidgets>
#include <QObject>

DeviceDBXMLReader::DeviceDBXMLReader(QWidget *parent)
    : QTreeWidget(parent)
{
    QStyle *style = this->style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileIcon));

}

bool DeviceDBXMLReader::read(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(device, true,&errorStr, &errorLine, &errorColumn)) {
        QMessageBox::information(window(), tr("Device Database"), tr("Parse Error at line %1, column %d:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if ((root.tagName() != "ManufacturerSpecificData") && (!root.hasAttribute("Revision"))) {
        QMessageBox::information(window(), tr("Device Database"),
                                 tr("The file is not an ManufacturerSpecific file."));
        return false;
    }

    clear();

    QDomElement child = root.firstChildElement("Manufacturer");
    while (!child.isNull()) {
        readManufacturer(child);
        child = child.nextSiblingElement("Manufacturer");
    }

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));

    return true;
}

bool DeviceDBXMLReader::write(QIODevice *device)
{
    const int IndentSize = 4;

    QTextStream out(device);
    domDocument.save(out, IndentSize);
    return true;
}

void DeviceDBXMLReader::updateDomElement(QTreeWidgetItem *item, int column)
{
#if 0
    QDomElement element = domElementForItem.value(item);
    if (!element.isNull()) {
        if (column == 0) {
            QDomElement oldTitleElement = element.firstChildElement("title");
            QDomElement newTitleElement = domDocument.createElement("title");

            QDomText newTitleText = domDocument.createTextNode(item->text(0));
            newTitleElement.appendChild(newTitleText);

            element.replaceChild(newTitleElement, oldTitleElement);
        } else {
            if (element.tagName() == "bookmark")
                element.setAttribute("href", item->text(1));
        }
    }
#endif
}



void DeviceDBXMLReader::readManufacturer(const QDomElement &element,
                                         QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = createItem(element, parentItem);

    QString title = element.attribute("name");

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setIcon(0, folderIcon);
    item->setText(0, title);

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "Product") {
            QTreeWidgetItem *childItem = createItem(child, item);

            QString title = child.attribute("name");

            childItem->setFlags(item->flags() | Qt::ItemIsEditable);
            childItem->setIcon(0, bookmarkIcon);
            childItem->setText(0, title);
            child = child.nextSiblingElement();
        }
    }

}
QTreeWidgetItem *DeviceDBXMLReader::createItem(const QDomElement &element,
                                      QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item;
    if (parentItem) {
        item = new QTreeWidgetItem(parentItem);
    } else {
        item = new QTreeWidgetItem(this);
    }
    domElementForItem.insert(item, element);
    return item;
}




void DeviceDBXMLReader::updateSelection() {

    QDomElement element = domElementForItem.value(this->currentItem());
    if (!element.isNull()) {
        if (element.nodeName().toUpper() == "MANUFACTURER") {
            qDebug() << "Loading Manufacturer Page for " << element.attribute("name");
            emit setupManufacturerPage(element);
        } else if (element.nodeName().toUpper() == "PRODUCT") {
            qDebug() << "Loading Product Page for " << element.attribute("name");
            emit setupProductPage(element);
        } else {
            qWarning() << "Unknown ManufacturerData Tag:" << element.nodeName();
        }
    }
}

#if 0
void DeviceDBXMLReader::setupManufacturerPage(QTreeWidgetItem *item) {
    this->tabWidget->setTabEnabled(3, false);
    this->tabWidget->setTabEnabled(2, false);
    this->tabWidget->setTabEnabled(1, false);
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>("ManufacturerSummary");
    if (pWidget) {
        QLineEdit *mname = this->tabWidget->findChild<QLineEdit *>("mname");
        if (mname)
            mname->setText(item->data(0, Manufacturer_Name).toString());
        QLineEdit *mid = this->tabWidget->findChild<QLineEdit *>("mid");
        if (mid)
            mid->setText(item->data(0, Manufacturer_ID).toString());
    } else {
        qDebug() << "Can't Find ManufacturerSummary Page";
    }



}

void DeviceDBXMLReader::setupProductPage(QTreeWidgetItem *item) {
    this->tabWidget->setTabEnabled(3, true);
    this->tabWidget->setTabEnabled(2, true);
    this->tabWidget->setTabEnabled(1, true);
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>("ManufacturerSummary");
    if (pWidget) {
        QLineEdit *mname = this->tabWidget->findChild<QLineEdit *>("mname");
        if (mname)
            mname->setText(item->data(0, Manufacturer_Name).toString());
        QLineEdit *mid = this->tabWidget->findChild<QLineEdit *>("mid");
        if (mid)
            mid->setText(item->data(0, Manufacturer_ID).toString());
    } else {
        qDebug() << "Can't Find ManufacturerSummary Page";
    }
    emit doProductPage(item);

}
#endif
