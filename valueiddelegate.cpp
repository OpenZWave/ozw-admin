#include <QLineEdit>
#include <QDebug>
#include "valueiddelegate.h"
#include "valueid.h"



ValueIDDelegate::ValueIDDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

void ValueIDDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    qDebug() << index.column();
    if (index.column() == 2) {
        QVariant value = index.model()->data(index, Qt::UserRole);
        QStyleOptionViewItem myOption = option;
        myOption.state &= ~QStyle::State_Enabled;
        QItemDelegate::paint(painter, myOption, index);
        return;
    }

    QItemDelegate::paint(painter, option, index);
}

QWidget *ValueIDDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem & /* option */,
        const QModelIndex &index) const
{
    if (index.column() != 2)
        return 0;

    QVariant originalValue = index.model()->data(index, Qt::UserRole);
//    if (!isSupportedType(originalValue.type()))
//        return 0;

    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(false);

    return lineEdit;
}

void ValueIDDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
        lineEdit->setText(displayText(value));
}

void ValueIDDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (!lineEdit->isModified())
        return;

    QString text = lineEdit->text();

    QVariant originalValue = index.model()->data(index, Qt::DisplayRole);
    QVariant value;

}
QString ValueIDDelegate::displayText(const QVariant &value)
{
    qDebug() << "Display" << value.typeName();
    QtValueID *vid = qvariant_cast<QtValueID *>(value);
    qDebug() << vid->getValue();
    switch (vid->getValue().type()) {
    case QVariant::Bool:
    case QVariant::ByteArray:
    case QVariant::Char:
    case QVariant::Double:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::String:
    case QVariant::UInt:
    case QVariant::ULongLong:
        return vid->getValue().toString();
    }

    return QString("returning <%1>").arg(vid->getValue().typeName());
}
