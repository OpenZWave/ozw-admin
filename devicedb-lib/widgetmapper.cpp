#include "widgetmapper.h"
#include <QtWidgets>
#include <QDebug>


WidgetMapper::WidgetMapper(QObject *parent) :
    QObject(parent)
{
    this->signalMapper = new QSignalMapper(this);
    connect(this->signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(WidgetChanged(QWidget *)));
}

bool WidgetMapper::MapWidgetValue(QWidget *widget, QDomElement *data) {
    RemoveWidget(widget);
    this->map.append(WidgetData(widget, data));
    ConnectSignals(widget);
    Populate(widget);
    return true;
}

bool WidgetMapper::MapWidgetAttribute(QWidget *widget, QDomElement *data, QString attribname) {
    RemoveWidget(widget);
    this->map.append(WidgetData(widget, data, attribname));
    ConnectSignals(widget);
    Populate(widget);
    return true;
}


bool WidgetMapper::ConnectSignals(QWidget *w) {
    if (QLineEdit *qle = qobject_cast<QLineEdit *>(w)) {
        connect(qle, SIGNAL(textEdited(const QString)), this->signalMapper, SLOT(map()));
        this->signalMapper->setMapping(qle, qle);
        return true;
    }
    return false;
}

bool WidgetMapper::RemoveWidget(QWidget *w) {
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

void WidgetMapper::WidgetChanged(QWidget *w) {
    int idx = FindWidget(w);
    this->map[idx].modified = true;
    qDebug() << w->metaObject()->className() << " Changed";
    qDebug() << FindWidget(w);
    emit Changed();
}

bool WidgetMapper::Populate(QWidget *w) {
    int idx = FindWidget(w);
    if (idx == -1)
        return false;
    WidgetData wd = this->map.at(idx);
    QVariant data;
    if (wd.attribname.length() > 0)
        data = wd.xmlnode.toElement().attribute(wd.attribname);
    else
        data = wd.xmlnode.toElement().text();
    wd.modified = false;
    if (QLineEdit *qle = qobject_cast<QLineEdit *>(wd.widget)) {
        qle->setText(data.toString());
        return true;
    }
    return false;
}

bool WidgetMapper::ResetWidgets() {
    for (int i = 0; i < this->map.count(); ++i) {
        Populate(this->map.at(i).widget);
        this->map[i].modified = false;
    }
    emit dataReset();
    return true;
}

QVariant WidgetMapper::GetWidgetValue(QWidget *w) {
    if (QLineEdit *qle = qobject_cast<QLineEdit *>(w))
        return qle->text();
    return QVariant();
}

bool WidgetMapper::CommitWidgets() {
    for (int i = 0; i < this->map.count(); ++i) {
        if (this->map.at(i).modified == true) {
            if (this->map.at(i).attribname.length() > 0) {
                this->map[i].xmlnode.toElement().setAttribute(this->map.at(i).attribname, GetWidgetValue(this->map.at(i).widget).toString());
                qDebug() << this->map[i].xmlnode.toElement().attribute(this->map.at(i).attribname);
            } else {
                this->map[i].xmlnode.toElement().setNodeValue(GetWidgetValue(this->map.at(i).widget).toString());
            }
            this->map[i].modified = false;
            qDebug() << this->map[i].xmlnode.ownerDocument().toString();
        }
    }
    emit dataSaved();
    return true;
}

