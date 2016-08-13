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
