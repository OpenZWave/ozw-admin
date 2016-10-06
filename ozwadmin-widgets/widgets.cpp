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
#include "widgets.h"
#include "ui_HelpEditorDlg.h"
#include "ui_ListDialog.h"

ValueTypeComboBox::ValueTypeComboBox(QWidget *parent) : QComboBox(parent) {
    /* setup the possible list types */
    insertItem(0, "Int", "int");
    insertItem(0, "Short", "short");
    insertItem(0, "Bool", "bool");
    insertItem(0, "Byte", "byte");
    insertItem(0, "Decimal", "decimal");
    insertItem(0, "List", "list");
    insertItem(0, "Schedule", "schedule");
    insertItem(0, "String", "string");
    insertItem(0, "Button", "button");
    insertItem(0, "Raw", "raw");
    setInsertPolicy(QComboBox::NoInsert);
}


HelpEditorDlg::HelpEditorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog())
{
    this->ui->setupUi(this);
}
void HelpEditorDlg::setHelpText(const QString text) {
    this->ui->plainTextEdit->setPlainText(text);
}

QString HelpEditorDlg::getHelpText() {
    return this->ui->plainTextEdit->toPlainText();
}

ValueHelpPopup::ValueHelpPopup(QWidget *parent) : QPushButton(parent) {
    QPushButton::setText("Help");
    connect(this, SIGNAL(released()), this, SLOT(activateHelpDlg()));
}
void ValueHelpPopup::setValueElement(QDomElement element) {
    QDomElement helpchild = element.firstChildElement("Help");
    if (!helpchild.isNull())
        helptext = helpchild.text();
}

QString ValueHelpPopup::getHelpText() {
    return helptext;
}

void ValueHelpPopup::activateHelpDlg() {
    HelpEditorDlg *hed = new HelpEditorDlg(this);
    hed->setHelpText(helptext);
    if (hed->exec() == QDialog::Accepted) {
        helptext = hed->getHelpText();
    }
}

BoolValueCheckBox::BoolValueCheckBox(QWidget *parent):
    QCheckBox(parent)
{

}

void BoolValueCheckBox::setBoolValue(QString value) {
    if (value.toUpper() == "TRUE")
        setChecked(true);
    else
        setChecked(false);
}

QString BoolValueCheckBox::getBoolValue() {
    if (isChecked())
        return "true";
    else
        return "false";

}

ListValueDlg::ListValueDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListDialog())
{
    this->ui->setupUi(this);
    this->ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers << "Name" << "Value";
    this->ui->tableWidget->setHorizontalHeaderLabels(headers);
    this->ui->tableWidget->verticalHeader()->hide();

}

void ListValueDlg::setValueElement(QDomElement element) {
    QDomElement itemchild = element.firstChildElement("Item");
    while (!itemchild.isNull()) {
        this->ui->tableWidget->insertRow(this->ui->tableWidget->rowCount());
        QTableWidgetItem *label = new QTableWidgetItem(itemchild.attribute("label"));
        this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount()-1,0, label);
        QTableWidgetItem *value = new QTableWidgetItem(itemchild.attribute("value"));
        this->ui->tableWidget->setItem(this->ui->tableWidget->rowCount()-1,1, value);
        itemchild = itemchild.nextSiblingElement();
    }
}

ValueListPopup::ValueListPopup(QWidget *parent) : QPushButton(parent) {
    QPushButton::setText("List Values");
    connect(this, SIGNAL(released()), this, SLOT(activateListDlg()));
}
void ValueListPopup::setValueElement(QDomElement listelement) {
    this->element = listelement;
}


void ValueListPopup::activateListDlg() {
    ListValueDlg *lvd = new ListValueDlg(this);
    lvd->setValueElement(this->element);
    if (lvd->exec() == QDialog::Accepted) {
        qDebug() << "Accepted";
    }
}
