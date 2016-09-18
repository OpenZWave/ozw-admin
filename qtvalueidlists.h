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
#ifndef QTVALUEIDLISTS_H
#define QTVALUEIDLISTS_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

#include "valueid.h"

enum QtValueIDColumnNames {
    QVID_NodeID,
    QVID_Label,
    QVID_QtValueID,
    QVID_Genre,
    QVID_CommandClass,
    QVID_Instance,
    QVID_Index,
    QVID_Type,
    QVID_Count
};

enum ValueFilter {
    VF_None = 0,
    VF_Genre_User = OpenZWave::ValueID::ValueGenre_User,
    VF_Genre_System = OpenZWave::ValueID::ValueGenre_System,
    VF_Genre_Config = OpenZWave::ValueID::ValueGenre_Config
};

class QtValueIDList : public QAbstractTableModel {
    Q_OBJECT
public:
    QtValueIDList(QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
    QList< QPair<QString, QString> > getList();

    void addQtValueID(QtValueID *);

private:

    QList<QtValueID *> m_QtValueIDlist;
};


class ValueIdFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ValueIdFilterProxyModel(QObject *parent = 0);
    void setFilterType (ValueFilter vf);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;
//    bool lessThan(const QModelIndex &left, const QModelIndex &right) const Q_DECL_OVERRIDE;

private:
    ValueFilter m_valueFilterType;
};


#endif // QTVALUEIDLISTS_H
