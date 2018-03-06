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
#ifndef VALUEID_H
#define VALUEID_H

#include <QObject>
#include <QHash>
#include <QAbstractTableModel>

#include "value_classes/ValueID.h"

class QtValueID : public QObject
{
    Q_OBJECT
public:
    QtValueID(const OpenZWave::ValueID &vid);
    QtValueID();
    QtValueID(const QtValueID &other);
    ~QtValueID();

    uint32 GetHomeId() const;
    uint32 GetNodeId() const;
    OpenZWave::ValueID::ValueGenre GetGenre() const;
    uint8 GetCommmandClassId() const;
    uint8 GetInstance() const;
    uint8 GetIndex() const;
    uint8 GetType() const;
    OpenZWave::ValueID const &getValueID() const;
    QVariant getValue();
    QStringList GetValueListItems() const;
    QString GetLabel() const;


signals:

public slots:
private:
    OpenZWave::ValueID m_vid;
};

Q_DECLARE_METATYPE(QtValueID)


#endif // VALUEID_H
