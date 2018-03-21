#include <QLineEdit>
#include <QCheckBox>
#include <QDebug>
#include <QApplication>
#include "valueiddelegate.h"
#include "valueid.h"
#include "bitsetwidget.h"

QString bit_array_to_string(const QBitArray& array);

ValueIDDelegate::ValueIDDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void ValueIDDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    if (index.column() == 2) {
        QVariant value = index.model()->data(index, Qt::DisplayRole);
        QtValueID *vid = qvariant_cast<QtValueID *>(value);
        if (vid->GetType() == OpenZWave::ValueID::ValueType_Bool) {
            this->paintCB(painter, option, index);
            return;
        }
    }

    QStyledItemDelegate::paint(painter, option, index);
}

void ValueIDDelegate::paintCB(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QtValueID *vid = qvariant_cast<QtValueID *>(value);
    bool data = vid->getValue().toBool();


    //create CheckBox style
    QStyleOptionButton checkboxstyle;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    //center
    checkboxstyle.rect = option.rect;
    checkboxstyle.rect.setLeft(option.rect.x() +
                               option.rect.width()/2 - checkbox_rect.width()/2);
    //checked or not checked
    if(data)
        checkboxstyle.state = QStyle::State_On|QStyle::State_Enabled;
    else
        checkboxstyle.state = QStyle::State_Off|QStyle::State_Enabled;

    //done! we can draw!
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);
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

    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QtValueID *vid = qvariant_cast<QtValueID *>(value);
    QWidget *widget;

    if (vid->GetType() == OpenZWave::ValueID::ValueType_BitSet) {
        BitSetWidget *cb = new BitSetWidget(parent);
        // BitSetWidget *cb = new BitSetWidget();
        //cb->setFrame(false);
        widget = cb;
    } else {
        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setFrame(false);
        widget = lineEdit;
    }
    QAbstractItemModel *model = (QAbstractItemModel *)index.model();
    model->setData(index, widget->sizeHint(), Qt::UserRole+100);
    return widget;
}

void ValueIDDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    QtValueID *vid = qvariant_cast<QtValueID *>(value);
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
        lineEdit->setText(displayText(value, QLocale()));
    if (BitSetWidget *bs = qobject_cast<BitSetWidget *>(editor))
        bs->setValue(vid);
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
QString ValueIDDelegate::displayText(const QVariant &value, const QLocale& locale ) const
{
    QtValueID *vid = qvariant_cast<QtValueID *>(value);
    //qDebug() << "Display: " << vid->GetType();
    //qDebug() << "value: " << vid->getValue();
    switch (vid->GetType()) {
    case OpenZWave::ValueID::ValueType_Bool:
    case OpenZWave::ValueID::ValueType_Button: {
        return vid->getValue().toBool() ? "true" : "false";
        break;
    }
    case OpenZWave::ValueID::ValueType_Byte: {
        bool ok;
        uint8 val = vid->getValue().toInt(&ok);
        if (ok) {
            return QString::number(val);
        } else {
            qWarning() << "byte Conversion Failed";
            return QString::number((uint8)0);
        }
        break;
    }
    case OpenZWave::ValueID::ValueType_Decimal: {
        bool ok;
        float val = vid->getValue().toFloat(&ok);
        if (ok) {
            return QString::number(val);
        } else {
            qWarning() << "Float Conversion Failed";
            return QString::number((float)0);
        }
        break;
    }
    case OpenZWave::ValueID::ValueType_Int: {
        bool ok;
        int val = vid->getValue().toInt(&ok);
        if (ok) {
            return QString::number(val);
        } else {
            qWarning() << "Int Conversion Failed";
            return QString::number((int)0);
        }
        break;
    }
    case OpenZWave::ValueID::ValueType_List: {
        bool ok;
        int val = vid->getValue().toInt(&ok);
        if (ok) {
            return QString::number(val);
        } else {
            qWarning() << "Int Conversion Failed";
            return QString::number((int)0);
        }
        break;
    }
    case OpenZWave::ValueID::ValueType_Schedule: {
        qWarning() << "schedule todo";
        break;
    }
    case OpenZWave::ValueID::ValueType_Short: {
        bool ok;
        short val = (short)vid->getValue().toInt(&ok);
        if (ok) {
            return QString::number(val);
        } else {
            qWarning() << "Short Conversion Failed";
            return QString::number((short)0);
        }
        break;
    }
    case OpenZWave::ValueID::ValueType_String: {
        return vid->getValue().toString();
        break;
    }

    case OpenZWave::ValueID::ValueType_Raw: {
        return vid->getValue().toString();
        break;
    }
    case OpenZWave::ValueID::ValueType_BitSet: {
        qDebug() << vid->GetBitMask();
        qDebug() << vid->getValue();
        qDebug() << bit_array_to_string(vid->getValue().toBitArray());
        return bit_array_to_string(vid->getValue().toBitArray());
        break;
    }
    }
    qWarning() << "Unhandled ValueID Type";
    return QString("returning <%1>").arg(vid->getValue().typeName());
}


void ValueIDDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    //    if (index.column() == 2) {

    //    }

    editor->setGeometry(option.rect);
}

QSize ValueIDDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << "size";
    if (index.column() == 2) {
        return index.model()->data(index, Qt::UserRole+100).toSize();
    }
}


QString bit_array_to_string(const QBitArray& array)
{
    QString str;
    for(int i = 0;i < array.size();i++)
        str += (array[i] ? "1" : "0");

    return str;
}
