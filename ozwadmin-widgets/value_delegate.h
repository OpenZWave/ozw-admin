#ifndef VALUE_DELEGATE_H
#define VALUE_DELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLoggingCategory>


Q_DECLARE_LOGGING_CATEGORY(ozwvd);


class Value_Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Value_Delegate(QObject *parent = nullptr);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;


private slots:
    void commitAndCloseEditor();
};

#endif // VALUE_DELEGATE_H
