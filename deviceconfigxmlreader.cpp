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
#include "deviceconfigxmlreader.h"

#include <QtWidgets>
#include <QObject>
#include "widgets.h"




DeviceConfigXMLReader::DeviceConfigXMLReader(QTabWidget *myTabWidget, QWidget *parent)
    : tabWidget(myTabWidget), QObject(parent)
{

}

void DeviceConfigXMLReader::setupManufacturerPage(const QDomElement &element) {
    this->tabWidget->setTabEnabled(3, false);
    this->tabWidget->setTabEnabled(2, false);
    this->tabWidget->setTabEnabled(1, false);
    this->tabWidget->setCurrentIndex(0);
    setFieldsFromElement(element, "ManufacturerSummary", "mname", "name");
    setFieldsFromElement(element, "ManufacturerSummary", "mid", "id");





}

void DeviceConfigXMLReader::setupProductPage(const QDomElement &element) {
    this->tabWidget->setTabEnabled(3, true);
    this->tabWidget->setTabEnabled(2, true);
    this->tabWidget->setTabEnabled(1, true);
    this->tabWidget->setCurrentIndex(2);

    QDomElement parent = element.parentNode().toElement();
    if (parent.isNull()) {
        qWarning() << "Parent Node is Null";
        return;
    }

    setFieldsFromElement(parent, "ManufacturerSummary", "mname", "name");
    setFieldsFromElement(parent, "ManufacturerSummary", "mid", "id");
    setFieldsFromElement(element, "ProductSummary", "ptype", "type");
    setFieldsFromElement(element, "ProductSummary", "pid", "id");
    setFieldsFromElement(element, "ProductSummary", "pname", "name");
    setFieldsFromElement(element, "ProductSummary", "pconfig", "config");


    /* load the Device Config, if it exists */
    if (element.hasAttribute("config")) {
        QFile dcxml("config/" + element.attribute("config"));
        if (!dcxml.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(tabWidget, tr("Device Database"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(dcxml.fileName())
                                 .arg(dcxml.errorString()));
        } else {
            read(&dcxml);
        }
        dcxml.close();
    }
}

bool DeviceConfigXMLReader::setFieldsFromElement(const QDomElement &element, QString tab, QString field, QString elementname) {
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>(tab);
    if (pWidget) {
        QLineEdit *qle = this->tabWidget->findChild<QLineEdit *>(field);
        if (qle) {
            qle->setText(element.attribute(elementname));
        } else {
            qWarning() << "Can't find Field named " << field;
            return false;
        }
    } else {
        qWarning() << "Can't find Tab named " << tab;
        return false;
    }
    return true;
}



bool DeviceConfigXMLReader::read(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(device, true,&errorStr, &errorLine, &errorColumn)) {
        QMessageBox::information(tabWidget, tr("Device Database"), tr("Parse Error at line %1, column %d:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if ((root.tagName() != "Product") && (!root.hasAttribute("Revision"))) {
        QMessageBox::information(tabWidget, tr("Device Database"),
                                 tr("The file is not an Device Configuration file."));
        return false;
    }


    QDomElement child = root.firstChildElement();
    while (!child.isNull()) {
        if (child.nodeName().toUpper() == "COMMANDCLASS") {
            /* configuation Params */
            if (child.attribute("id") == "112") {
                doConfigurationParams(child);
            } else {
                qWarning() << "Unhandled CommandClass: " << child.attribute("id");
            }

        } else if (child.nodeName().toUpper() == "PROTOCOL") {

        } else {
            qWarning() << "Unknown Element in Config File: " << child.nodeName();
        }
        child = child.nextSiblingElement();
    }

    return true;
}

void DeviceConfigXMLReader::doConfigurationParams(const QDomElement &element) {
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>("ProductConfig");
    if (!pWidget) {
        qWarning() << "Can't find ProductConfig Tab";
        return;
    }
#if 0
    qDebug() << "About to delete Layout";
   // if (pWidget->layout())
   //     delete pWidget->layout();
    qDebug() << "About to delete widgets";
    QList<QWidget *> widgets = pWidget->findChildren<QWidget *>();
    foreach(QWidget * widget, widgets)
    {
        qDebug() << "Deleting " << widget->objectName() << widget->metaObject()->className();
                    widget->dumpObjectInfo();
    //    widget->deleteLater();
    }
    qDebug() << "About to add layout";
    QTableWidget *table = new QTableWidget(0, 10, pWidget);
    QStringList headers;
    headers << "Index" << "Type" << "Label" << "Help" << "Default" << "Read Only" << "Write Only";
    table->setHorizontalHeaderLabels(headers);
    table->verticalHeader()->hide();
#endif
    QTableWidget *table;
    if (!pWidget->layout()) {
        QHBoxLayout *Layout = new QHBoxLayout(pWidget);
        table = new QTableWidget(0, 10, pWidget);
        table->setObjectName("ConfigTable");
        QStringList headers;
        headers << "Index" << "Type" << "Label" << "Help" << "Default" << "Read Only" << "Write Only";
        table->setHorizontalHeaderLabels(headers);
        table->verticalHeader()->hide();
        Layout->addWidget(table);
    } else {
        table = pWidget->findChild<QTableWidget *>("ConfigTable");
        if (table) {
            table->clearContents();
            table->setRowCount(0);
        } else {
            qWarning() << "Can't find ConfigTable";
            return;
        }
    }
    qDebug() << "Now setting up table data";
    QDomElement value = element.firstChildElement();
    while (!value.isNull()) {
        if (value.nodeName().toUpper() == "VALUE") {
            qDebug() << "Inserting Row for Config Index " << value.attribute("index");
            table->insertRow(table->rowCount());
            QSpinBox *idx = new QSpinBox();
            idx->setMinimum(1);
            idx->setMaximum(255);
            idx->setValue(value.attribute("index").toInt());
            table->setCellWidget(table->rowCount()-1, 0, idx);

            ValueTypeComboBox *type = new ValueTypeComboBox();
            type->setCurrentIndex(type->findData(value.attribute("type")));
            table->setCellWidget(table->rowCount()-1, 1, type);

            QTableWidgetItem *label = new QTableWidgetItem(value.attribute("label"));
            table->setItem(table->rowCount()-1, 2, label);

            ValueHelpPopup *help = new ValueHelpPopup();
            help->setValueElement(value);
            table->setCellWidget(table->rowCount()-1, 3, help);

            QTableWidgetItem *defval = new QTableWidgetItem(value.attribute("value"));
            table->setItem(table->rowCount()-1,4, defval);

            BoolValueCheckBox *ro = new BoolValueCheckBox();
            ro->setText("ReadOnly");
            ro->setBoolValue(value.attribute("read_only"));
            table->setCellWidget(table->rowCount()-1, 5, ro);

            BoolValueCheckBox *wo = new BoolValueCheckBox();
            wo->setText("WriteOnly");
            wo->setBoolValue(value.attribute("write_only"));
            table->setCellWidget(table->rowCount()-1, 6, wo);

            if (value.attribute("type").toUpper() == "LIST") {
                ValueListPopup *vlp = new ValueListPopup();
                vlp->setValueElement(value);
                table->setCellWidget(table->rowCount()-1, 7, vlp);
            }

        }
        value = value.nextSiblingElement();
    }
}





bool DeviceConfigXMLReader::write(QIODevice *device)
{
    const int IndentSize = 4;

    QTextStream out(device);
    domDocument.save(out, IndentSize);
    return true;
}

#if 0
void DeviceConfigXMLReader::updateDomElement(QTreeWidgetItem *item, int column)
{
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
}
#endif


#if 0
void deviceconfigxmlreader::readManufacturer(const QDomElement &element,
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

#endif

#if 0
QTreeWidgetItem *deviceconfigxmlreader::createItem(const QDomElement &element,
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

#endif
