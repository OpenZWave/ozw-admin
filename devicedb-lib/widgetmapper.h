#ifndef WIDGETMAPPER_H
#define WIDGETMAPPER_H

#include <QObject>
#include <QMap>
#include <QWidget>
#include <QTableWidgetItem>
#include <QDomDocument>
#include <QSignalMapper>
#include <QPointer>

class WidgetMapper : public QObject
{
    Q_OBJECT
public:
    explicit WidgetMapper(QObject *parent = 0);
    bool MapWidgetValue(QWidget *, QDomNode &node);
    bool MapWidgetAttribute(QWidget *, QDomNode &node, QString attribname);
    bool MapWidgetValue(QTableWidgetItem *, QDomNode &node);
    bool MapWidgetAttribute(QTableWidgetItem *, QDomNode &node, QString attribname);
    bool RemoveWidget(QWidget *);
    bool RemoveWidget(QTableWidgetItem *);
    bool ResetWidgets();
    bool CommitWidgets();
signals:
    void Changed();
    void dataSaved();
    void dataReset();

private slots:
    void WidgetChanged(QWidget *);
    void WidgetChanged(QTableWidgetItem *);
private:
    struct WidgetData {
    public:
        enum WidgetType {
            WT_Widget,
            WT_TblWidget
        };
        inline WidgetData(QWidget *w = 0, QDomNode d = QDomNode()) : widget(w),  xmlnode(d), attribname(), modified(false), type(WT_Widget){ }
        inline WidgetData(QWidget *w, QDomNode d, QString a) : widget(w),  xmlnode(d), attribname(a), modified(false), type(WT_Widget) { }
        inline WidgetData(QTableWidgetItem *w = 0, QDomNode d = QDomNode()) : tblwidget(w),  xmlnode(d), attribname(), modified(false), type(WT_TblWidget) { }
        inline WidgetData(QTableWidgetItem *w, QDomNode d, QString a) : tblwidget(w),  xmlnode(d), attribname(a), modified(false), type(WT_TblWidget) { }
        QPointer<QWidget> widget;
        QTableWidgetItem *tblwidget;
        QDomNode xmlnode;
        QString attribname;
        bool modified;
        WidgetType type;
    };
    int FindWidget(QWidget *) const;
    int FindWidget(QTableWidgetItem *) const;
    bool ConnectSignals(QWidget *w);
    bool ConnectSignals(QTableWidgetItem *w);
    bool Populate(QWidget *w);
    bool Populate(QTableWidgetItem *w);
    QVariant GetWidgetValue(QWidget *w);
    QVariant GetWidgetValue(QTableWidgetItem *w);
    QList<WidgetData> map;
    QSignalMapper *signalMapper;
};

#endif // WIDGETMAPPER_H
