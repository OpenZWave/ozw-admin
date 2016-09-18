#ifndef VALUEIDDELEGATE_H
#define VALUEIDDELEGATE_H

#include <QItemDelegate>

class ValueIDDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ValueIDDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    static QString displayText(const QVariant &value);
};

#endif // VALUEIDDELEGATE_H
