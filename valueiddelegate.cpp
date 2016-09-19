#include <QLineEdit>
#include <QDebug>
#include <QApplication>
#include "valueiddelegate.h"
#include "valueid.h"



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

    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(false);

    return lineEdit;
}

void ValueIDDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor))
        lineEdit->setText(displayText(value, QLocale()));
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
    qDebug() << "Display: " << vid->GetType();
    qDebug() << "value: " << vid->getValue();
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
            qDebug() << "byte Conversion Failed";
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
            qDebug() << "Float Conversion Failed";
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
            qDebug() << "Int Conversion Failed";
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
            qDebug() << "Int Conversion Failed";
            return QString::number((int)0);
        }
        break;
    }
    case OpenZWave::ValueID::ValueType_Schedule: {
        qDebug() << "schedule todo";
        break;
    }
    case OpenZWave::ValueID::ValueType_Short: {
        bool ok;
        short val = (short)vid->getValue().toInt(&ok);
        if (ok) {
            return QString::number(val);
        } else {
            qDebug() << "Short Conversion Failed";
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
    }

    return QString("returning <%1>").arg(vid->getValue().typeName());
}
