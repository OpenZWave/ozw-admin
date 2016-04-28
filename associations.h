#ifndef ASSOCIATIONS_H
#define ASSOCIATIONS_H

#include <QObject>
#include <QHash>


class associationinfo : public QObject {
    Q_OBJECT
public:
    explicit associationinfo(qint8 GroupID, QObject *parent = 0);
    qint8 getGroupID();
    void addNode(qint8, qint8 instance);
    void removeNode(qint8);
    void resetMembers();
    bool isMember(qint8);
    qint8 getMaxAssociations();
    void setMaxAssociations(qint8);
    void setGroupName(QString);
    QString getGroupName();
private:
    qint8 m_groupID;
    QString m_name;
    qint8 m_maxassoc;
    QList<qint8> m_members;

};


class associations : public QObject
{
    Q_OBJECT
public:
    explicit associations(QObject *parent = 0);
    void setMaxGroups(qint8);
    qint8 getMaxGroups();
    associationinfo *addGroup(qint8);
    associationinfo *getGroup(qint8);
    void removeGroup(qint8);
    void addGroupMember(qint8, qint8, qint8 instance = 0);
    void removeGroupMember(qint8, qint8, qint8 instance = 0);
    bool isGroupMember(qint8, qint8);

signals:
    void groupUpdated(qint8);
public slots:
private:
    QHash<qint8, associationinfo *> m_groups;
    qint8 m_maxgroups;
};



#endif // ASSOCIATIONS_H
