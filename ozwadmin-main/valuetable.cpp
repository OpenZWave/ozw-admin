
#include <QHeaderView>
#include <qt-openzwave/qtopenzwave.h>
#include <qt-openzwave/qtozwmanager.h>

#include "valuetable.h"
#include "value_delegate.h"
#include "ozwcore.h"
#include "util.h"

ValueTable::ValueTable(QTOZW_ValueIds::ValueIdGenres genre, QWidget *parent) :
    QTableView(parent)
{
	Value_Delegate *delegate = new Value_Delegate(this);
    this->m_genre = genre;

    this->m_proxyModel = new QTOZW_proxyValueModel(this);    
    this->m_proxyModel->setFilterGenre(this->m_genre);

	this->setItemDelegateForColumn(QTOZW_ValueIds::ValueIdColumns::Value, delegate);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->verticalHeader()->hide();
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	this->setSortingEnabled(true);
	this->horizontalHeader()->setSectionsMovable(true);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void ValueTable::setModel(QAbstractItemModel *model, QItemSelectionModel *selectionModel)
{
    this->m_proxyModel->setSourceModel(model);
    this->m_proxyModel->setSelectionModel(selectionModel);
    connect(this->m_proxyModel, &QAbstractItemModel::rowsInserted, this, &ValueTable::resizeContents);
    connect(this->m_proxyModel, &QAbstractItemModel::rowsRemoved, this, &ValueTable::resizeContents);
    //connect(selectionModel, &QItemSelectionModel::currentRowChanged, this, &ValueTable::logChanges);
    QTableView::setModel(this->m_proxyModel);

    for (int i = 0; i <= QTOZW_ValueIds::ValueIdColumns::ValueIdCount; i++) {
        switch (i) {
            case QTOZW_ValueIds::ValueIdColumns::Label:
            case QTOZW_ValueIds::ValueIdColumns::Value:
            case QTOZW_ValueIds::ValueIdColumns::Instance:
            case QTOZW_ValueIds::ValueIdColumns::Help:
            break;
        default:
            this->horizontalHeader()->hideSection(i);
        }
    }
    QTimer::singleShot(500, this, &ValueTable::resizeContents);
}

void ValueTable::resizeContents() {
    this->resizeColumnsToContents();
}

void forEach(QAbstractItemModel* model, QModelIndex parent = QModelIndex()) {
    for(int r = 0; r < model->rowCount(parent); ++r) {
        qCDebug(ozwadmin) << "\t\t" << model->data(model->index(r, QTOZW_ValueIds::Node, parent)).toInt() <<  model->data(model->index(r, QTOZW_ValueIds::Genre, parent)).toInt();
    }
}



void ValueTable::logChanges(const QModelIndex &current, const QModelIndex &previous)
{
    qCDebug(ozwadmin) << "SelectionChanged UnFiltered Rows:" << this->m_proxyModel->sourceModel()->rowCount();
    qCDebug(ozwadmin) << "\t Filter Node:" << this->m_proxyModel->getCurrentFilteredNode();
    qCDebug(ozwadmin) << "\t Genre Filter:" << (int)this->m_proxyModel->getFilterGenre();
    qCDebug(ozwadmin) << "\t Filtered Rows: " << this->m_proxyModel->rowCount();
    qCDebug(ozwadmin) << "\t TableView Rows:" << QTableView::model()->rowCount();
    forEach(this->m_proxyModel->sourceModel());
}
