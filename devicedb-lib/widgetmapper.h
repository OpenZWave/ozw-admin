#ifndef WIDGETMAPPER_H
#define WIDGETMAPPER_H

#include <QObject>
#include <QMap>
#include <QWidget>
#include <QDomDocument>
#include <QSignalMapper>
#include <QPointer>

class WidgetMapper : public QObject
{
    Q_OBJECT
public:
    explicit WidgetMapper(QObject *parent = 0);
    bool MapWidgetValue(QWidget *, QDomElement *element);
    bool MapWidgetAttribute(QWidget *, QDomElement *element, QString attribname);
    bool RemoveWidget(QWidget *);
    bool ResetWidgets();
    bool CommitWidgets();
signals:
    void Changed();
    void dataSaved();
    void dataReset();

private slots:
    void WidgetChanged(QWidget *);
private:
    struct WidgetData {
    public:
        inline WidgetData(QWidget *w = 0, QDomNode *d = 0) : widget(w), attribname(), modified(false) { xmlnode = d->cloneNode(true);}
        inline WidgetData(QWidget *w, QDomNode *d, QString a) : widget(w), attribname(a), modified(false) { xmlnode = d->cloneNode(true);}
        QPointer<QWidget> widget;
        QDomNode xmlnode;
        QString attribname;
        bool modified;
    };
    int FindWidget(QWidget *) const;
    bool ConnectSignals(QWidget *w);
    bool Populate(QWidget *w);
    QVariant GetWidgetValue(QWidget *w);
    QList<WidgetData> map;
    QSignalMapper *signalMapper;
};

#endif // WIDGETMAPPER_H
