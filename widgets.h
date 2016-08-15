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
#ifndef WIDGETS_H
#define WIDGETS_H

#include <QtWidgets>
#include <QObject>
#include <QDomDocument>


class ValueTypeComboBox : public QComboBox {
    Q_OBJECT
public:
    ValueTypeComboBox(QWidget *parent = 0);
};

namespace Ui {
class Dialog;
class ListDialog;
}


class HelpEditorDlg : public QDialog {
    Q_OBJECT
public:
    HelpEditorDlg(QWidget *parent = 0);
    void setHelpText(const QString text);
    QString getHelpText();
private:
    Ui::Dialog *ui;
};


class ValueHelpPopup : public QPushButton {
    Q_OBJECT
public:
    ValueHelpPopup(QWidget *parent = 0);
    void setValueElement(QDomElement element);
    QString getHelpText();
public slots:
    void activateHelpDlg();

private:
    QString helptext;
};

class BoolValueCheckBox : public QCheckBox {
    Q_OBJECT
public:
    BoolValueCheckBox(QWidget *parent = 0);
    void setBoolValue(QString);
    QString getBoolValue();
};

class ListValueDlg : public QDialog {
    Q_OBJECT
public:
    ListValueDlg(QWidget *parent = 0);
    void setValueElement(QDomElement element);
private:
    Ui::ListDialog *ui;
};

class ValueListPopup : public QPushButton {
    Q_OBJECT
public:
    ValueListPopup(QWidget *parent = 0);
    void setValueElement(QDomElement element);
public slots:
    void activateListDlg();

private:
    QDomElement element;
};

#endif // WIDGETS_H
