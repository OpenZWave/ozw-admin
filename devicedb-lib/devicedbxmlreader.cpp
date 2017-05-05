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

    if (!domDocument.setContent(device, false,&errorStr, &errorLine, &errorColumn)) {
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
    qDebug() << "doing Children";
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



void DeviceDBXMLReader::readManufacturer(const QDomNode &node,
                                         QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = createItem(node, parentItem);
    if (!node.isElement()) {
        qWarning() << "Node is not a Element: " << node.nodeName();
        return;
    }

    QString title = node.toElement().attribute("name");

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setIcon(0, folderIcon);
    item->setText(0, title);

    QDomElement child = node.firstChildElement();
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
QTreeWidgetItem *DeviceDBXMLReader::createItem(const QDomNode &node,
                                      QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item;
    if (parentItem) {
        item = new QTreeWidgetItem(parentItem);
    } else {
        item = new QTreeWidgetItem(this);
    }
    domNodeForItem.insert(item, node);
    return item;
}




void DeviceDBXMLReader::updateSelection() {

    QDomNode node = domNodeForItem.value(this->currentItem());
    if (!node.isElement()) {
        qWarning() << "Node is not a Element:" << node.nodeName();
        return;
    }
    if (!node.isNull()) {
        if (node.nodeName().toUpper() == "MANUFACTURER") {
            qDebug() << "Loading Manufacturer Page for " << node.toElement().attribute("name");
            emit setupManufacturerPage(node);
        } else if (node.nodeName().toUpper() == "PRODUCT") {
            qDebug() << "Loading Product Page for " << node.toElement().attribute("name");
            emit setupProductPage(node);
        } else {
            qWarning() << "Unknown ManufacturerData Tag:" << node.nodeName();
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
void DeviceDBXMLReader::dump() {
    qDebug() << domDocument.toString();
}
