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

#include <QDebug>
#include "nodes.h"
#include "Manager.h"
#include "Group.h"


Node::Node(qint8 m_nodeid, int homeid)
{

    this->m_nodeid = m_nodeid;
    this->m_homeid = homeid;
}

qint8 Node::getNodeID() const {
    return this->m_nodeid;
}

QString Node::getNodeName() const {
    return OpenZWave::Manager::Get()->GetNodeName(this->m_homeid, this->m_nodeid).c_str();
}
void Node::setNodeName(QString name) {
    if (this->getNodeName() != name) {
        OpenZWave::Manager::Get()->SetNodeName(this->m_homeid, this->m_nodeid, name.toStdString());
        emit NodeNameChanged(name);
    }
}
QString Node::getNodeLocation() const {
    return OpenZWave::Manager::Get()->GetNodeLocation(this->m_homeid, this->m_nodeid).c_str();
}
void Node::setNodeLocation(QString location) {
    if (this->getNodeLocation() != location) {
        OpenZWave::Manager::Get()->SetNodeLocation(this->m_homeid, this->m_nodeid, location.toStdString());
        emit NodeLocationChanged(location);
    }
}
QString Node::getNodeManufacturer() const {
    return OpenZWave::Manager::Get()->GetNodeManufacturerName(this->m_homeid, this->m_nodeid).c_str();
}
QString Node::getNodeProduct() const {
    return OpenZWave::Manager::Get()->GetNodeProductName(this->m_homeid, this->m_nodeid).c_str();
}
QString Node::getNodeBasicType() const {
    if (OpenZWave::Manager::Get()->IsNodeZWavePlus(this->m_homeid, this->m_nodeid)) {
        QString mystr(OpenZWave::Manager::Get()->GetNodePlusTypeString(this->m_homeid, this->m_nodeid).c_str());
        mystr.append(" ");
        mystr.append(OpenZWave::Manager::Get()->GetNodeRoleString(this->m_homeid, this->m_nodeid).c_str());
        return mystr;
    } else {
        return nodeBasicStr(OpenZWave::Manager::Get()->GetNodeBasic(this->m_homeid, this->m_nodeid));
    }
}
QString Node::getNodeGenericType() const {
    if (OpenZWave::Manager::Get()->IsNodeZWavePlus(this->m_homeid, this->m_nodeid)) {
        return OpenZWave::Manager::Get()->GetNodeDeviceTypeString(this->m_homeid, this->m_nodeid).c_str();
    } else {
        return OpenZWave::Manager::Get()->GetNodeType(this->m_homeid, this->m_nodeid).c_str();
    }
}
bool Node::getIsZWPlus() const {
    return OpenZWave::Manager::Get()->IsNodeZWavePlus(this->m_homeid, this->m_nodeid);
}
bool Node::getIsListening() const {
    return OpenZWave::Manager::Get()->IsNodeListeningDevice(this->m_homeid, this->m_nodeid);
}
bool Node::getIsBeaming() const {
    return OpenZWave::Manager::Get()->IsNodeBeamingDevice(this->m_homeid, this->m_nodeid);
}
bool Node::getIsRouting() const {
    return OpenZWave::Manager::Get()->IsNodeRoutingDevice(this->m_homeid, this->m_nodeid);
}
bool Node::getIsFLiRS() const {
    return OpenZWave::Manager::Get()->IsNodeFrequentListeningDevice(this->m_homeid, this->m_nodeid);
}
bool Node::getIsSecurity() const {
    return OpenZWave::Manager::Get()->IsNodeSecurityDevice(this->m_homeid, this->m_nodeid);
}
bool Node::getIsNodeFailed() const {
    return OpenZWave::Manager::Get()->IsNodeFailed(this->m_homeid, this->m_nodeid);
}
bool Node::getIsNodeAwake() const {
    return OpenZWave::Manager::Get()->IsNodeAwake(this->m_homeid, this->m_nodeid);
}
QString Node::getNodeProductID() const {
    return OpenZWave::Manager::Get()->GetNodeProductId(this->m_homeid, this->m_nodeid).c_str();
}
QString Node::getNodeProductType() const {
    return OpenZWave::Manager::Get()->GetNodeProductType(this->m_homeid, this->m_nodeid).c_str();
}
QString Node::getNodeZWVersion() const {
    return QString::number(OpenZWave::Manager::Get()->GetNodeVersion(this->m_homeid, this->m_nodeid));
}
QString Node::getNodeBaudRate() const {
    return QString::number(OpenZWave::Manager::Get()->GetNodeMaxBaudRate(this->m_homeid, this->m_nodeid));
}
QString Node::getNodeQueryStage() const {
    return OpenZWave::Manager::Get()->GetNodeQueryStage(this->m_homeid, this->m_nodeid).c_str();
}

OpenZWave::Node::NodeData &Node::getNodeStatistics() {
    OpenZWave::Manager::Get()->GetNodeStatistics(this->m_homeid, this->m_nodeid, &this->m_stats);
    return this->m_stats;
}

QString Node::getNodeMetaData(OpenZWave::Node::MetaDataFields field) const {\
    return OpenZWave::Manager::Get()->GetMetaData(this->m_homeid, this->m_nodeid, field).c_str();
}

void Node::updateGroups() {
    int numGroups = OpenZWave::Manager::Get()->GetNumGroups(this->m_homeid, this->m_nodeid);
    for (int i = 1; i <= numGroups; i++) {
        associationinfo *group = this->m_groups.getGroup(i);
        if (!group) {
            group = this->m_groups.addGroup(i);
            group->setGroupName(OpenZWave::Manager::Get()->GetGroupLabel(this->m_homeid, this->m_nodeid, i).c_str());
            group->setMaxAssociations(OpenZWave::Manager::Get()->GetMaxAssociations(this->m_homeid, this->m_nodeid, i));
        }
        group->resetMembers();
        OpenZWave::InstanceAssociation *v = NULL;
        int num = OpenZWave::Manager::Get()->GetAssociations(this->m_homeid, this->m_nodeid, i, &v);
        for (int j = 0; j < num; j++) {
            group->addNode(v[j].m_nodeId, v[j].m_instance);
        }

        
    }
}

qint8 Node::getNumGroups() {
    return OpenZWave::Manager::Get()->GetNumGroups(this->m_homeid, this->m_nodeid);
}

associationinfo *Node::getGroup(qint8 groupID) {
    return this->m_groups.getGroup(groupID);
}

QtValueIDList *Node::getValues(ValueFilter vf) {
    switch (vf) {
        case VF_None: {
            return &this->m_valueids;
            break;
        }
    case VF_Genre_User: {
        return &this->m_valueids;
        break;
    }
    case VF_Genre_System: {
        return &this->m_valueids;
        break;
    }
    case VF_Genre_Config: {
        return &this->m_valueids;
        break;
    }
    }


}



void Node::ValueAdded(QtValueID *vid) {
    this->m_valueids.addQtValueID(vid);
}




NodeList::NodeList(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int NodeList::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return this->m_Nodelist.size();
}
int NodeList::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return NCN_Count;
}
QVariant NodeList::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= this->m_Nodelist.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        Node *node = this->m_Nodelist.at(index.row());
        if (!node) {
            return QVariant();
        }
        switch ((NodeColumnNames)index.column()) {
        case NCN_NodeID:
            return node->getNodeID();
        case NCN_NodeName:
            return node->getNodeName();
        case NCN_NodeLocation:
            return node->getNodeLocation();
        case NCN_NodeManufacturerName:
            return node->getNodeManufacturer();
        case NCN_NodeProductName:
            return node->getNodeProduct();
        case NCN_NodeBasicType:
            return node->getNodeBasicType();
        case NCN_NodeGenericType:
            return node->getNodeGenericType();
        case NCN_QueryStage:
            return node->getNodeQueryStage();
        case NCN_Count:
            return QVariant();
        }
    }
    return QVariant();

}
QVariant NodeList::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch ((NodeColumnNames)section) {
        case NCN_NodeID:
            return tr("NodeID");

        case NCN_NodeName:
            return tr("Node Name");

        case NCN_NodeLocation:
            return tr("Location");

        case NCN_NodeManufacturerName:
            return tr("Manufacturer");

        case NCN_NodeProductName:
            return tr("Product");

        case NCN_NodeBasicType:
            return tr("Basic Type");

        case NCN_NodeGenericType:
            return tr("Generic Type");

        case NCN_QueryStage:
            return tr("Query Stage");

        case NCN_Count:
            return QVariant();

        }
    }
    return QVariant();
}
Qt::ItemFlags NodeList::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool NodeList::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        Node *p = this->m_Nodelist.at(row);

        switch ((NodeColumnNames)index.column()) {
        case NCN_NodeID:
        case NCN_NodeManufacturerName:
        case NCN_NodeProductName:
        case NCN_NodeBasicType:
        case NCN_NodeGenericType:
        case NCN_QueryStage:
        case NCN_Count:
            /* read only */
            return false;
            break;
        case NCN_NodeName:
            p->setNodeName(value.toString());
            break;
        case NCN_NodeLocation:
            p->setNodeLocation(value.toString());
            break;
        }
    } else {
        return false;
    }
    emit(dataChanged(index, index));

    return true;
}

bool NodeList::insertRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(position);
    Q_UNUSED(rows);
    Q_UNUSED(index);

    return false;
}

bool NodeList::removeRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(position);
    Q_UNUSED(rows);
    Q_UNUSED(index);

    return false;
}





void NodeList::addNode(Node *node) {
    this->m_Nodelist.append(node);
    beginInsertRows(QModelIndex(), this->m_Nodelist.size(), this->m_Nodelist.size());
    endInsertRows();
}

Node* NodeList::getNode(qint8 nodeid) {
    QListIterator<Node *> i(this->m_Nodelist);
    while (i.hasNext()) {
        Node *node = i.next();
        if (node->getNodeID() == nodeid)
            return node;
    }
    return NULL;
}

QModelIndex NodeList::getNodeValueIndex(qint8 nodeid, NodeColumnNames value) {
    int j = 0;
    QListIterator<Node *> i(this->m_Nodelist);
    while (i.hasNext()) {
        Node *node = i.next();
        if (node->getNodeID() == nodeid)
            break;
        j++;
    }
    return this->index(j, (int)value);
}

void NodeList::updateQueryStage(qint8 node) {
    QModelIndex i = getNodeValueIndex(node, NCN_QueryStage);
    if (!i.isValid()) {
        qWarning() << "Can't update QueryStage for Node "<< node;
        return;
    }
    emit(dataChanged(i, i));
}

void NodeList::updateGroups(qint8 nodeid) {
    Node *node = getNode(nodeid);
    if (!node) {
        qWarning() << "Can't find Node " << nodeid;
        return;
    }
    node->updateGroups();
}

void NodeList::ValueAdded(QtValueID *vid) {
    Node *node = getNode(vid->GetNodeId());
    if (!node) {
        qWarning() << "Can't find Node " << vid->GetNodeId();
        return;
    }
    node->ValueAdded(vid);
}
