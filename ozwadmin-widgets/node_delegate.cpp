#include <QPainter>
#include <QDebug>
#include <QApplication>

#include <qt-openzwave/qtozwnodemodel.h>

#include "node_delegate.h"
#include "nodeflagswidget.h"

Node_Delegate::Node_Delegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

void Node_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NodeFlagsWidget fw;
    fw.setValue(index.data().value<QBitArray>());
    fw.setGeometry(option.rect);
    painter->save();
    painter->translate(option.rect.topLeft());
    fw.render(painter, QPoint(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}

QSize Node_Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    NodeFlagsWidget fw;
    fw.setValue(index.data().value<QBitArray>());
    return fw.sizeHint();
}

