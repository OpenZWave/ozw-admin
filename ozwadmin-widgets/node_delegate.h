#ifndef NODE_DELEGATE_H
#define NODE_DELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

class Node_Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Node_Delegate(QObject *parent = nullptr);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // NODE_DELEGATE_H
