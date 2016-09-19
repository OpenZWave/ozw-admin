#ifndef VALUEIDDELEGATE_H
#define VALUEIDDELEGATE_H

#include <QStyledItemDelegate>

class ValueIDDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ValueIDDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void paintCB(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QString displayText(const QVariant &value, const QLocale& locale ) const;
};

#endif // VALUEIDDELEGATE_H
