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


#ifndef NODES_H
#define NODES_H

#include <QObject>
#include <QHash>
#include <QAbstractTableModel>
#include "Manager.h"
#include "util.h"





class Node : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(qint8 NodeID READ getNodeID)
    Q_PROPERTY(QString NodeName READ getNodeName WRITE setNodeName NOTIFY NodeNameChanged)
    Q_PROPERTY(QString NodeLocation READ getNodeLocation WRITE setNodeLocation NOTIFY NodeLocationChanged)
    Q_PROPERTY(QString NodeManufacturer READ getNodeManufacturer)
    Q_PROPERTY(QString NodeProduct READ getNodeProduct)
    Q_PROPERTY(QString NodeBasicType READ getNodeBasicType)
    Q_PROPERTY(QString NodeGenericType READ getNodeGenericType)
    Q_PROPERTY(bool ZWPlus READ getIsZWPlus)


    Node(qint8 m_nodeid, int homeid);

    qint8 getNodeID() const { return this->m_nodeid; }
    QString getNodeName() const { return OpenZWave::Manager::Get()->GetNodeName(this->m_homeid, this->m_nodeid).c_str(); }
    void setNodeName(QString name) {
        if (this->getNodeName() != name) {
            OpenZWave::Manager::Get()->SetNodeName(this->m_homeid, this->m_nodeid, name.toStdString());
            emit NodeNameChanged(name);
        }
    }
    QString getNodeLocation() const { return OpenZWave::Manager::Get()->GetNodeLocation(this->m_homeid, this->m_nodeid).c_str(); }
    void setNodeLocation(QString location) {
        if (this->getNodeLocation() != location) {
            OpenZWave::Manager::Get()->SetNodeLocation(this->m_homeid, this->m_nodeid, location.toStdString());
            emit NodeLocationChanged(location);
        }
    }
    QString getNodeManufacturer() const { return OpenZWave::Manager::Get()->GetNodeManufacturerName(this->m_homeid, this->m_nodeid).c_str(); }
    QString getNodeProduct() const { return OpenZWave::Manager::Get()->GetNodeProductName(this->m_homeid, this->m_nodeid).c_str(); }
    QString getNodeBasicType() const {
        if (OpenZWave::Manager::Get()->IsNodeZWavePlus(this->m_homeid, this->m_nodeid)) {
            QString mystr(OpenZWave::Manager::Get()->GetNodePlusTypeString(this->m_homeid, this->m_nodeid).c_str());
            mystr.append(" ");
            mystr.append(OpenZWave::Manager::Get()->GetNodeRoleString(this->m_homeid, this->m_nodeid).c_str());
            return mystr;
        } else {
            return nodeBasicStr(OpenZWave::Manager::Get()->GetNodeBasic(this->m_homeid, this->m_nodeid));
        }
    }
    QString getNodeGenericType() const {
        if (OpenZWave::Manager::Get()->IsNodeZWavePlus(this->m_homeid, this->m_nodeid)) {
            return OpenZWave::Manager::Get()->GetNodeDeviceTypeString(this->m_homeid, this->m_nodeid).c_str();
        } else {
            return OpenZWave::Manager::Get()->GetNodeType(this->m_homeid, this->m_nodeid).c_str();
        }
    }
    bool getIsZWPlus() const {
        return OpenZWave::Manager::Get()->IsNodeZWavePlus(this->m_homeid, this->m_nodeid);
    }



signals:
    void NodeNameChanged(QString);
    void NodeLocationChanged(QString);

private:
    qint8 m_nodeid;
    int m_homeid;
};


enum NodeColumnNames {
    NCN_NodeID,
    NCN_NodeName,
    NCN_NodeLocation,
    NCN_NodeManufacturerName,
    NCN_NodeProductName,
    NCN_NodeBasicType,
    NCN_NodeGenericType,
    NCN_Count
};


class NodeList : public QAbstractTableModel {
    Q_OBJECT
public:
    NodeList(QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
    QList< QPair<QString, QString> > getList();

    void addNode(Node *);
    Node *getNode(qint8);
    QModelIndex getNodeValueIndex(qint8, NodeColumnNames);


private:

    QList<Node *> m_Nodelist;
};


extern NodeList *ozwNodes;


#endif // NODES_H
