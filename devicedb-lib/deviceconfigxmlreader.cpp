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
#include "commandclasslist.h"
#include "widgetmapper.h"



DeviceConfigXMLReader::DeviceConfigXMLReader(QTabWidget *myTabWidget, QWidget *parent)
    : tabWidget(myTabWidget), QObject(parent)
{
    this->wm = new WidgetMapper(this);
    connect(this->wm, SIGNAL(Changed()), this, SLOT(formDataChanged()));
    connect(this->wm, SIGNAL(dataReset()), this, SLOT(p_dataWasReset()));
    connect(this->wm, SIGNAL(dataSaved()), this, SLOT(p_dataWasSaved()));
}

void DeviceConfigXMLReader::setupManufacturerPage(QDomNode &node) {
    this->tabWidget->setTabEnabled(3, false);
    this->tabWidget->setTabEnabled(2, false);
    this->tabWidget->setTabEnabled(1, false);
    this->tabWidget->setCurrentIndex(0);
    setFieldsFromElement(node, "ManufacturerSummary", "mname", "name");
    setFieldsFromElement(node, "ManufacturerSummary", "mid", "id");
}

void DeviceConfigXMLReader::setupProductPage(QDomNode &node) {
    this->tabWidget->setTabEnabled(3, true);
    this->tabWidget->setTabEnabled(2, true);
    this->tabWidget->setTabEnabled(1, true);

    if (!node.isElement()) {
        qWarning() << "Node is not a Element: " << node.nodeName();
        return;
    }


    QDomNode parent = node.parentNode();
    if (parent.isNull()) {
        qWarning() << "Parent Node is Null";
        return;
    }

    setFieldsFromElement(parent, "ManufacturerSummary", "mname", "name");
    setFieldsFromElement(parent, "ManufacturerSummary", "mid", "id");
    setFieldsFromElement(node, "ProductSummary", "ptype", "type");
    setFieldsFromElement(node, "ProductSummary", "pid", "id");
    setFieldsFromElement(node, "ProductSummary", "pname", "name");
    setFieldsFromElement(node, "ProductSummary", "pconfig", "config");


    /* load the Device Config, if it exists */
    if (node.toElement().hasAttribute("config")) {
        QFile dcxml(this->m_Path + "/" + node.toElement().attribute("config"));
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

bool DeviceConfigXMLReader::setFieldsFromElement(QDomNode &node, QString tab, QString field, QString elementname) {
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>(tab);
    if (pWidget) {
        QLineEdit *qle = this->tabWidget->findChild<QLineEdit *>(field);
        if (qle) {
            this->wm->MapWidgetAttribute(qle, node, elementname);
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



    /* clear the Quirks Table */
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>("ProductQuirks");
    if (!pWidget) {
        qWarning() << "Can't find ProductQuirks Tab";
        return false;
    }

    QTableWidget* tbl= pWidget->findChild<QTableWidget *>("pq_tableWidget");
    if (!tbl) {
        qWarning() << "Can't find pq_tableWidget";
        return false;
    }
    tbl->clearContents();
    tbl->setRowCount(0);

    QDomNodeList children = root.childNodes();
    for (int i = 0; i < children.size(); ++i) {
        QDomElement child = children.item(i).toElement();
        if (child.nodeName().toUpper() == "COMMANDCLASS") {
            /* configuation Params */
            if (child.attribute("id") == "112") {
                doConfigurationParams(child);
            } else if (child.attribute("id") == "133") {
                doAssociations(child);
            } else {
                doQuirks(child);
            }

        } else if (child.nodeName().toUpper() == "PROTOCOL") {
            qWarning() << "Handle Protocol Block";
        } else if (child.nodeName().toUpper() == "METADATA") {
            doMetaData(child);
        }else {
            qWarning() << "Unknown Element in Config File: " << child.nodeName();
        }
    }
    return true;
}

void DeviceConfigXMLReader::doConfigurationParams(QDomNode &node) {
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>("ProductConfig");
    if (!pWidget) {
        qWarning() << "Can't find ProductConfig Tab";
        return;
    }

    if (!node.isElement()) {
        qWarning() << "Node is not a Element: " << node.nodeName();
        return;
    }

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
    QDomElement value = node.firstChildElement();
    while (!value.isNull()) {
        if (value.nodeName().toUpper() == "VALUE") {
            qDebug() << "Inserting Row for Config Index " << value.attribute("index");
            table->insertRow(table->rowCount());
            QSpinBox *idx = new QSpinBox();
            idx->setMinimum(1);
            idx->setMaximum(255);
            idx->setValue(value.attribute("index").toInt());
            table->setCellWidget(table->rowCount()-1, 0, idx);
            this->wm->MapWidgetAttribute(idx,value,"index");

            ValueTypeComboBox *type = new ValueTypeComboBox();
            type->setCurrentIndex(type->findData(value.attribute("type")));
            table->setCellWidget(table->rowCount()-1, 1, type);
            this->wm->MapWidgetAttribute(type,value,"type");

            QTableWidgetItem *label = new QTableWidgetItem(value.attribute("label"));
            table->setItem(table->rowCount()-1, 2, label);
            this->wm->MapWidgetAttribute(label, value, "label");

            ValueHelpPopup *help = new ValueHelpPopup();
            help->setValueElement(value);
            table->setCellWidget(table->rowCount()-1, 3, help);
            this->wm->MapWidgetAttribute(help, value, "help");

            QTableWidgetItem *defval = new QTableWidgetItem(value.attribute("value"));
            table->setItem(table->rowCount()-1,4, defval);
            this->wm->MapWidgetAttribute(defval, value, "value");

            BoolValueCheckBox *ro = new BoolValueCheckBox();
            ro->setText("ReadOnly");
            ro->setBoolValue(value.attribute("read_only"));
            table->setCellWidget(table->rowCount()-1, 5, ro);
            this->wm->MapWidgetAttribute(ro, value, "read_only");

            BoolValueCheckBox *wo = new BoolValueCheckBox();
            wo->setText("WriteOnly");
            wo->setBoolValue(value.attribute("write_only"));
            table->setCellWidget(table->rowCount()-1, 6, wo);
            this->wm->MapWidgetAttribute(wo, value, "write_only");

            if (value.attribute("type").toUpper() == "LIST") {
                ValueListPopup *vlp = new ValueListPopup();
                vlp->setValueElement(value);
                table->setCellWidget(table->rowCount()-1, 7, vlp);
                this->wm->MapWidgetAttribute(vlp, value, "value");
            }

        }
        value = value.nextSiblingElement();
    }
}

void DeviceConfigXMLReader::doAssociations(QDomNode &node)
{
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>("ProductAssociations");
    if (!pWidget) {
        qWarning() << "Can't find ProductAssociations Tab";
        return;
    }
    if (!node.isElement()) {
        qWarning() << "Node is not a Element: " << node.nodeName();
        return;
    }


    qDebug() << "Now setting up table data";
    QDomElement value = node.firstChildElement();
    while (!value.isNull()) {
        qDebug() << value.nodeName().toUpper();
        if (value.nodeName().toUpper() == "ASSOCIATIONS") {
            setFieldsFromElement(value, "ProductAssociations", "pc_maxassociations", "num_groups");
        }
        QTableWidget *table = this->tabWidget->findChild<QTableWidget *>("AssocTable");
        if (!table) {
            table = new QTableWidget(0, 4, pWidget);
            table->setObjectName("AssocTable");
            QStringList headers;
            headers << "Index" <<  "Label" << "Max Nodes" << "Auto Associate";
            table->setHorizontalHeaderLabels(headers);
            table->verticalHeader()->hide();
            qDebug() << pWidget->layout()->objectName();
            QFormLayout *lo = qobject_cast<QFormLayout *>(pWidget->layout());
            if (lo) {
                lo->addRow(table);
            } else {
                QFormLayout *lo = new QFormLayout(pWidget);
                lo->addRow(table);
            }
        } else {
            table->clearContents();
            table->setRowCount(0);
        }
        qDebug() << "Now setting up table data";

        QDomElement assoc = value.firstChildElement("Group");
        while (!assoc.isNull()) {
            qDebug() << "Doing Group " << assoc.attribute("label");
            qDebug() << "Inserting Row for Association Index " << assoc.attribute("index");
            table->insertRow(table->rowCount());
            QSpinBox *idx = new QSpinBox();
            idx->setMinimum(1);
            idx->setMaximum(255);
            idx->setValue(assoc.attribute("index").toInt());
            table->setCellWidget(table->rowCount()-1, 0, idx);
            this->wm->MapWidgetAttribute(idx, assoc, "index");

            QTableWidgetItem *label = new QTableWidgetItem(assoc.attribute("label"));
            table->setItem(table->rowCount()-1, 1, label);
            this->wm->MapWidgetAttribute(label, assoc, "label");

            QSpinBox *ma = new QSpinBox();
            ma->setMinimum(1);
            ma->setMaximum(255);
            ma->setValue(assoc.attribute("max_associations").toInt());
            table->setCellWidget(table->rowCount()-1, 2, ma);
            this->wm->MapWidgetAttribute(ma,assoc, "max_associations");


            BoolValueCheckBox *ro = new BoolValueCheckBox();
            ro->setText("Auto Associate");
            if (assoc.attribute("index").toInt() == 1)
                ro->setBoolValue(assoc.attribute("auto", "true"));
            else
                ro->setBoolValue(assoc.attribute("auto", "false"));
            table->setCellWidget(table->rowCount()-1, 3, ro);
            this->wm->MapWidgetAttribute(ro, assoc, "auto");

            assoc = assoc.nextSiblingElement("Group");
        }
        value = value.nextSiblingElement();
    }
}


void DeviceConfigXMLReader::doQuirks(QDomNode &node)
{
    QWidget* pWidget= this->tabWidget->findChild<QWidget *>("ProductQuirks");
    if (!pWidget) {
        qWarning() << "Can't find ProductQuirks Tab";
        return;
    }

    QTableWidget* tbl= pWidget->findChild<QTableWidget *>("pq_tableWidget");
    if (!tbl) {
        qWarning() << "Can't find pq_tableWidget";
        return;
    }

    if (!node.isElement()) {
        qWarning() << "Node is not a Element: " << node.nodeName();
        return;
    }

    QDomNamedNodeMap attribs = node.toElement().attributes();
    qDebug() << attribs.namedItem("id").nodeValue();
    int CC = attribs.namedItem("id").nodeValue().toInt();
    if (CC == 0) {
        qDebug() << "No CC Info";
        //return;
    }
    CommandClassList ccl;
    for (int i = 0; i < attribs.count(); i++) {
        QDomNode node = attribs.item(i);
        if (node.nodeName().toUpper() == "ID")
            continue;
        tbl->insertRow(tbl->rowCount());

        QTableWidgetItem *ccname = new QTableWidgetItem(ccl.getName(CC));
        tbl->setItem(tbl->rowCount()-1, 0, ccname);

        QTableWidgetItem *label = new QTableWidgetItem(node.nodeName());
        tbl->setItem(tbl->rowCount()-1, 1, label);

        QTableWidgetItem *value = new QTableWidgetItem(node.nodeValue());
        tbl->setItem(tbl->rowCount()-1, 2, value);
        //this->wm->MapWidgetValue(value, node);

    }
}

void DeviceConfigXMLReader::doMetaData(QDomNode &node) {
    if (!node.isElement()) {
        qWarning() << "Node is not a Element" << node.nodeName();
        return;
    }
    QDomElement child = node.firstChildElement();
    while (!child.isNull()) {
        if (child.nodeName().toUpper() == "METADATAITEM") {
            qDebug() << child.attributes().namedItem("name").nodeValue();
            qDebug() << child.text();
        }
        child = child.nextSiblingElement();
    }
}
bool DeviceConfigXMLReader::write(QIODevice *device)
{
    const int IndentSize = 4;

    QTextStream out(device);
    domDocument.save(out, IndentSize);
    return true;
}

void DeviceConfigXMLReader::setPath(QString path)
{
    this->m_Path = path;
}

void DeviceConfigXMLReader::formDataChanged() {
    emit changed();
}

void DeviceConfigXMLReader::saveData() {
    this->wm->CommitWidgets();
}

void DeviceConfigXMLReader::resetData() {
    this->wm->ResetWidgets();
}

void DeviceConfigXMLReader::p_dataWasSaved() {
    emit dataWasSaved();
    qDebug() << domDocument.toString();
}

void DeviceConfigXMLReader::p_dataWasReset() {
    emit dataWasReset();
}
