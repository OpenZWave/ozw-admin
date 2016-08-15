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
#include "associations.h"

associations::associations(QObject *parent) : QObject(parent)
{

}

void associations::setMaxGroups(qint8 max) {
    this->m_maxgroups = max;
}

qint8 associations::getMaxGroups() {
    return this->m_maxgroups;
}

associationinfo *associations::addGroup(qint8 groupID) {
    associationinfo *group = new associationinfo(groupID, this);
    this->m_groups.insert(groupID, group);
    emit groupUpdated(groupID);
    return group;
}

associationinfo *associations::getGroup(qint8 groupID) {
    if (this->m_groups.find(groupID) != this->m_groups.end())
        return this->m_groups.value(groupID);
    else
        return NULL;
}

void associations::removeGroup(qint8 groupID) {
    if (this->m_groups.find(groupID) != this->m_groups.end()) {
        associationinfo *group = this->m_groups.value(groupID);
        delete group;
        emit groupUpdated(groupID);
        return;
    }
    qWarning() << "Can't Find Group " << groupID << " in Group List";
}

void associations::addGroupMember(qint8 groupID, qint8 nodeID, qint8 instance) {
    if (instance > 0) qWarning() << "Instance Associations Not Handled Yet";
    if (this->m_groups.find(groupID) != this->m_groups.end()) {
        associationinfo *group = this->m_groups.value(groupID);
        group->addNode(nodeID,instance);
        emit groupUpdated(groupID);
        return;
    }
    qWarning() << "Can't find Group " << groupID << " in GroupList for AddGroupMember";
}

void associations::removeGroupMember(qint8 groupID, qint8 nodeID, qint8 instance) {
    if (instance > 0) qWarning() << "Instance Associations Not Handled Yet";
    if (this->m_groups.find(groupID) != this->m_groups.end()) {
        associationinfo *group = this->m_groups.value(groupID);
        group->addNode(nodeID, instance);
        emit groupUpdated(groupID);
        return;
    }
    qWarning() << "Can't find Group " << groupID << " in GroupList for AddGroupMember";

}

bool associations::isGroupMember(qint8 groupID, qint8 nodeID) {
    if (this->m_groups.find(groupID) != this->m_groups.end()) {
        associationinfo *group = this->m_groups.value(groupID);
        return group->isMember(nodeID);
    }
    qWarning() << "Can't find Group " << groupID << " in GroupList for isGroupMember";
    return false;
}



associationinfo::associationinfo(qint8 GroupID, QObject *parent) : QObject(parent) {
    this->m_groupID = GroupID;
}

qint8 associationinfo::getGroupID() {
    return this->m_groupID;
}

void associationinfo::addNode(qint8 node, qint8 instance) {
    /* check its not in the list already */
    if ( instance > 0) qWarning() << "Instance Assocations Not Handled Yet";
    if (this->m_members.indexOf(node) > 0) {
        qWarning() << "Node Already a Member of Association Group " << this->m_groupID << " (" << this->m_name << ")";
        return;
    }
    this->m_members.append(node);
}

void associationinfo::removeNode(qint8 node) {
    if (this->m_members.indexOf(node) > 0) {
        this->m_members.removeAll(node);
        return;
    }
    qWarning() << "Node Not a Member of Association Group " << this->m_groupID << " (" << this->m_name << ")";
}

void associationinfo::resetMembers() {
    this->m_members.clear();
}

bool associationinfo::isMember(qint8 node) {
    if (this->m_members.indexOf(node) > 0)
        return true;
    return false;
}


qint8 associationinfo::getMaxAssociations() {
    return this->m_maxassoc;

}

void associationinfo::setMaxAssociations(qint8 max) {
    this->m_maxassoc = max;
}

void associationinfo::setGroupName(QString name) {
    this->m_name = name;
}

QString associationinfo::getGroupName() {
    return this->m_name;

}

