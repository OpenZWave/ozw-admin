#include "widgetmapper.h"
#include "widgets.h"
#include <QtWidgets>
#include <QDebug>


WidgetMapper::WidgetMapper(QObject *parent) :
    QObject(parent)
{
    this->signalMapper = new QSignalMapper(this);
    connect(this->signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(WidgetChanged(QWidget *)));
}

bool WidgetMapper::MapWidgetValue(QWidget *widget, QDomNode &data) {
    RemoveWidget(widget);
    this->map.append(WidgetData(widget, data));
    ConnectSignals(widget);
    Populate(widget);
    return true;
}

bool WidgetMapper::MapWidgetValue(QTableWidgetItem *tblwidget, QDomNode &data) {
    RemoveWidget(tblwidget);
    this->map.append(WidgetData(tblwidget, data));
    ConnectSignals(tblwidget);
    Populate(tblwidget);
    return true;
}

bool WidgetMapper::MapWidgetAttribute(QWidget *widget, QDomNode &data, QString attribname) {
    RemoveWidget(widget);
    this->map.append(WidgetData(widget, data, attribname));
    ConnectSignals(widget);
    Populate(widget);
    return true;
}


bool WidgetMapper::MapWidgetAttribute(QTableWidgetItem *tblwidget, QDomNode &data, QString attribname) {
    RemoveWidget(tblwidget);
    this->map.append(WidgetData(tblwidget, data, attribname));
    ConnectSignals(tblwidget);
    Populate(tblwidget);
    return true;
}

bool WidgetMapper::ConnectSignals(QWidget *w) {
    if (QLineEdit *qle = qobject_cast<QLineEdit *>(w)) {
        connect(qle, SIGNAL(textEdited(const QString)), this->signalMapper, SLOT(map()));
        this->signalMapper->setMapping(qle, qle);
        return true;
    } else if (QSpinBox *qsb = qobject_cast<QSpinBox *>(w)) {
        connect(qsb, SIGNAL(valueChanged(int)), this->signalMapper, SLOT(map()));
        this->signalMapper->setMapping(qsb, qsb);
        return true;
    } else if (BoolValueCheckBox *bcb = qobject_cast<BoolValueCheckBox *>(w)) {
        connect(bcb, SIGNAL(stateChanged(int)), this->signalMapper, SLOT(map()));
        this->signalMapper->setMapping(bcb, bcb);
        return true;
    }
    qWarning() << "No Support for " << w->metaObject()->className() << " in ConnectSignals";
    return false;
}

bool WidgetMapper::ConnectSignals(QTableWidgetItem *w) {
    connect(w->tableWidget(), SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(WidgetChanged(QTableWidgetItem*)));
    return true;
}
bool WidgetMapper::RemoveWidget(QWidget *w) {
    int idx = FindWidget(w);
    if (idx == -1)
        return false;
    this->map.removeAt(idx);
    this->signalMapper->removeMappings(w);
    return true;
}

bool WidgetMapper::RemoveWidget(QTableWidgetItem *w) {
    int idx = FindWidget(w);
    if (idx == -1)
        return false;
    this->map.removeAt(idx);
    return true;
}

int WidgetMapper::FindWidget(QWidget *w) const {
    for (int i = 0; i < this->map.count(); ++i) {
        if (this->map.at(i).widget == w)
            return i;
    }
    return -1;
}

int WidgetMapper::FindWidget(QTableWidgetItem *w) const {
    for (int i = 0; i < this->map.count(); ++i) {
        if (this->map.at(i).tblwidget == w)
            return i;
    }
    return -1;
}
void WidgetMapper::WidgetChanged(QWidget *w) {
    int idx = FindWidget(w);
    this->map[idx].modified = true;
    qDebug() << w->metaObject()->className() << " Changed";
    qDebug() << FindWidget(w);
    emit Changed();
}

void WidgetMapper::WidgetChanged(QTableWidgetItem *w) {
    int idx = FindWidget(w);
    this->map[idx].modified = true;
    qDebug() << this->map[idx].xmlnode.nodeName() << " Changed";
    qDebug() << FindWidget(w);
    emit Changed();
}

bool WidgetMapper::Populate(QWidget *w) {
    int idx = FindWidget(w);
    if (idx == -1)
        return false;
    WidgetData wd = this->map.at(idx);
    if (wd.type != WidgetData::WT_Widget) {
        qWarning() << "Not a Widget: " << wd.xmlnode.nodeName();
        return false;
    }
    QVariant data;
    if (wd.attribname.length() > 0)
        data = wd.xmlnode.toElement().attribute(wd.attribname);
    else
        data = wd.xmlnode.toElement().text();
    wd.modified = false;
    if (QLineEdit *qle = qobject_cast<QLineEdit *>(wd.widget)) {
        qle->setText(data.toString());
        return true;
    } else if (QSpinBox *qsb = qobject_cast<QSpinBox *>(wd.widget)) {
        qsb->setValue(data.toInt());
        return true;
    } else if (BoolValueCheckBox *bcb = qobject_cast<BoolValueCheckBox *>(wd.widget)) {
        bcb->setBoolValue(data.toString());
        return true;
    }
    qWarning() << "No Support for " << w->metaObject()->className() << " in Populate";
    return false;
}

bool WidgetMapper::Populate(QTableWidgetItem *w) {
    int idx = FindWidget(w);
    if (idx == -1)
        return false;
    WidgetData wd = this->map.at(idx);
    if (wd.type != WidgetData::WT_TblWidget) {
        qWarning() << "Not a TableWidget: " << wd.xmlnode.nodeName();
        return false;
    }
    QVariant data;
    if (wd.attribname.length() > 0)
        data = wd.xmlnode.toElement().attribute(wd.attribname);
    else
        data = wd.xmlnode.toElement().text();
    wd.modified = false;
    w->setText(data.toString());
    return false;
}

bool WidgetMapper::ResetWidgets() {
    for (int i = 0; i < this->map.count(); ++i) {
        if (this->map.at(i).type == WidgetData::WT_Widget)
            Populate(this->map.at(i).widget);
        else if (this->map.at(i).type == WidgetData::WT_TblWidget)
            Populate(this->map.at(i).tblwidget);

        this->map[i].modified = false;
    }
    emit dataReset();
    return true;
}

QVariant WidgetMapper::GetWidgetValue(QWidget *w) {
    if (QLineEdit *qle = qobject_cast<QLineEdit *>(w))
        return qle->text();
    else if (QSpinBox *qsb = qobject_cast<QSpinBox *>(w))
        return qsb->value();
    else if (BoolValueCheckBox *bcb = qobject_cast<BoolValueCheckBox *>(w))
        return bcb->getBoolValue();
    qWarning() << "No Support for " << w->metaObject()->className() << " in GetWidgetValue";
    return QVariant();
}

QVariant WidgetMapper::GetWidgetValue(QTableWidgetItem *w) {
    return w->text();
}
bool WidgetMapper::CommitWidgets() {
    for (int i = 0; i < this->map.count(); ++i) {
        if (this->map.at(i).modified == true) {
            if (this->map.at(i).attribname.length() > 0) {
                if (this->map.at(i).type == WidgetData::WT_Widget)
                    this->map[i].xmlnode.toElement().setAttribute(this->map.at(i).attribname, GetWidgetValue(this->map.at(i).widget).toString());
                else if (this->map.at(i).type == WidgetData::WT_TblWidget)
                    this->map[i].xmlnode.toElement().setAttribute(this->map.at(i).attribname, GetWidgetValue(this->map.at(i).tblwidget).toString());
            } else {
                if (this->map.at(i).type == WidgetData::WT_Widget)
                    this->map[i].xmlnode.toElement().setNodeValue(GetWidgetValue(this->map.at(i).widget).toString());
                else if (this->map.at(i).type == WidgetData::WT_TblWidget)
                    this->map[i].xmlnode.toElement().setNodeValue(GetWidgetValue(this->map.at(i).tblwidget).toString());
            }
            this->map[i].modified = false;
        }
    }
    emit dataSaved();
    return true;
}

