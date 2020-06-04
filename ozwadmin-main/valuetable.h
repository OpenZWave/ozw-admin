#ifndef VALUETABLE_H
#define VALUETABLE_H

#include <QTableView>
#include <qt-openzwave/qtozwproxymodels.h>

class ValueTable : public QTableView
{
public:
    ValueTable(QTOZW_ValueIds::ValueIdGenres genre, QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model, QItemSelectionModel *selectionModel);
public slots:

private slots:
    void resizeContents();
    void logChanges();
private:
    QTOZW_proxyValueModel *m_proxyModel;
    QTOZW_ValueIds::ValueIdGenres m_genre;
};

#endif // VALUETABLE_H
