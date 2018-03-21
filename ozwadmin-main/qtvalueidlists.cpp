/*  ozw-admin - Gui for OpenZWave
 *    Copyright (C) 2016  Justin Hammond <justin@dynam.ac>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qtvalueidlists.h"
#include <QDebug>

#include "Manager.h"

QtValueIDList::QtValueIDList(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int QtValueIDList::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return this->m_QtValueIDlist.size();
}
int QtValueIDList::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return QVID_Count;
}
QVariant QtValueIDList::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= this->m_QtValueIDlist.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        QtValueID *vid = this->m_QtValueIDlist.at(index.row());
        if (!vid) {
            return QVariant();
        }

        switch ((QtValueIDColumnNames)index.column()) {
        case QVID_NodeID:
            return vid->GetNodeId();
        case QVID_Label:
            return QString::fromStdString(OpenZWave::Manager::Get()->GetValueLabel(vid->getValueID()));
        case QVID_QtValueID:
            return QVariant::fromValue(vid);
        case QVID_Genre:
            return vid->GetGenre();
        case QVID_CommandClass:
            return vid->GetCommmandClassId();
        case QVID_Instance:
            return vid->GetInstance();
        case QVID_Index:
            return vid->GetIndex();
        case QVID_Type:
            return vid->GetType();
        case QVID_Count:
            return QVariant();
        }
    }
    return QVariant();

}
QVariant QtValueIDList::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch ((QtValueIDColumnNames)section) {
        case QVID_NodeID:
            return tr("NodeID");

        case QVID_Label:
            return tr("Label");

        case QVID_QtValueID:
            return tr("QtValudID");

        case QVID_Genre:
            return tr("Genre");

        case QVID_CommandClass:
            return tr("CommandClass");

        case QVID_Instance:
            return tr("Instance");

        case QVID_Index:
            return tr("Index");

        case QVID_Type:
            return tr("Type");

        case QVID_Count:
            return QVariant();

        }
    }
    return QVariant();
}
Qt::ItemFlags QtValueIDList::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool QtValueIDList::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        QtValueID *vid = this->m_QtValueIDlist.at(row);

        switch ((QtValueIDColumnNames)index.column()) {
        case QVID_NodeID:
        case QVID_Label:
        case QVID_Genre:
        case QVID_CommandClass:
        case QVID_Instance:
        case QVID_Index:
        case QVID_Type:
        case QVID_Count:
            /* read only */
            return false;
            break;
        case QVID_QtValueID:
            // XXX TODO

            break;
        }
    } else {
        return false;
    }
    emit(dataChanged(index, index));

    return true;
}

bool QtValueIDList::insertRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(position);
    Q_UNUSED(rows);
    Q_UNUSED(index);

    return false;
}

bool QtValueIDList::removeRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(position);
    Q_UNUSED(rows);
    Q_UNUSED(index);

    return false;
}

void QtValueIDList::addQtValueID(QtValueID *vid) {
    //qDebug() << "Adding Value";
    this->m_QtValueIDlist.append(vid);
    beginInsertRows(QModelIndex(), this->m_QtValueIDlist.size(), this->m_QtValueIDlist.size());
    endInsertRows();
}

ValueIdFilterProxyModel::ValueIdFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent),
      m_valueFilterType(VF_None)
{
}

void ValueIdFilterProxyModel::setFilterType (ValueFilter vf) {
    this->m_valueFilterType = vf;
}

bool ValueIdFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    if (this->m_valueFilterType == VF_None)
        return true;
    QModelIndex index0 = sourceModel()->index(sourceRow, QVID_Genre, sourceParent);
    if (sourceModel()->data(index0).toInt() == this->m_valueFilterType)
        return true;
    /* else, return false */
    return false;
}



