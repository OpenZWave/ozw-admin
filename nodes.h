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
#include "associations.h"





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

    Q_PROPERTY(bool IsZWPlus READ getIsZWPlus)
    Q_PROPERTY(bool IsListening READ getIsListening)
    Q_PROPERTY(bool IsBeaming READ getIsBeaming)
    Q_PROPERTY(bool IsRouting READ getIsRouting)
    Q_PROPERTY(bool IsFLiRS READ getIsFLiRS)
    Q_PROPERTY(bool IsSecurity READ getIsSecurity)
    Q_PROPERTY(bool IsNodeFailed READ getIsNodeFailed)
    Q_PROPERTY(bool IsNodeAwake READ getIsNodeAwake)


    Q_PROPERTY(QString NodeProductID READ getNodeProductID)
    Q_PROPERTY(QString NodeProductType READ getNodeProductType)
    Q_PROPERTY(QString NodeZWVersion READ getNodeZWVersion)
    Q_PROPERTY(QString NodeBaudRate READ getNodeBaudRate)
    Q_PROPERTY(QString NodeQueryStage READ getNodeQueryStage)




    Node(qint8 m_nodeid, int homeid);

    qint8 getNodeID() const;
    QString getNodeName() const;
    void setNodeName(QString name);
    QString getNodeLocation() const;
    void setNodeLocation(QString location);
    QString getNodeManufacturer() const;
    QString getNodeProduct() const;
    QString getNodeBasicType() const;
    QString getNodeGenericType() const;
    bool getIsZWPlus() const;
    bool getIsListening() const;
    bool getIsBeaming() const;
    bool getIsRouting() const;
    bool getIsFLiRS() const;
    bool getIsSecurity() const;
    bool getIsNodeFailed() const;
    bool getIsNodeAwake() const;
    QString getNodeProductID() const;
    QString getNodeProductType() const;
    QString getNodeZWVersion() const;
    QString getNodeBaudRate() const;
    QString getNodeQueryStage() const;
    OpenZWave::Node::NodeData &getNodeStatistics();


    void updateGroups();
    associationinfo *getGroup(qint8);
    qint8 getNumGroups();

signals:
    void NodeNameChanged(QString);
    void NodeLocationChanged(QString);

private:
    qint8 m_nodeid;
    int m_homeid;
    OpenZWave::Node::NodeData m_stats;
    associations m_groups;
};


enum NodeColumnNames {
    NCN_NodeID,
    NCN_NodeName,
    NCN_NodeLocation,
    NCN_NodeManufacturerName,
    NCN_NodeProductName,
    NCN_NodeBasicType,
    NCN_NodeGenericType,
    NCN_QueryStage,
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
    void updateQueryStage(qint8);
    void updateGroups(qint8);

private:

    QList<Node *> m_Nodelist;
};


extern NodeList *ozwNodes;


#endif // NODES_H
