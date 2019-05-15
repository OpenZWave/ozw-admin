#ifndef QTOZWMANAGER_H
#define QTOZWMANAGER_H

#include <QObject>
#include "rep_qtozwmanager_source.h"
#include "qtozwnotification.h"
#include "qtozwnodemodel.h"
#include "qtozwvalueidmodel.h"

void setupOZW();

enum QTOZW_UserRoles {
    ModelDataChanged = Qt::UserRole
};


namespace OpenZWave {

/* forward Declaration */
class Manager;
class Options;
class Notification;
}

class QTOZWManager : public QTOZWManagerSimpleSource
{
public:
    friend class OZWNotification;
    QTOZWManager();

    bool Start(QString SerialPort);
    QTOZW_Nodes *getNodeModel();
    QTOZW_ValueIds *getValueModel();

    bool refreshNodeInfo(uint8_t _node);
    bool requestNodeState(uint8_t _node);
    bool requestNodeDynamic(uint8_t _node);

    bool setConfigParam(uint8_t _node, uint8_t _param, int32_t _value, uint8_t const _size);
    void requestConfigParam(uint8_t _node, uint8_t _param);
    void requestAllConfigParam(uint8_t _node);

    void softResetController();
    void hardResetController();

    bool cancelControllerCommand();

    void testNetworkNode(uint8_t _node, uint32_t const _count);
    void testNetwork(uint32_t const _count);
    void healNetworkNode(uint8_t _node, bool _doRR);
    void healNetwork(bool _doRR);
    bool addNode(bool _doSecure);
    bool removeNode();
    bool removeFailedNode(uint8_t _node);
    bool hasNodeFailed(uint8_t _node);
    bool requestNodeNeighborUpdate(uint8_t _node);
    bool assignReturnRoute(uint8_t _node);
    bool deleteAllReturnRoute(uint8_t _node);
    bool sendNodeInfomation(uint8_t _node);
    bool replaceFailedNode(uint8_t _node);
    bool requestNetworkUpdate(uint8_t _node);

    bool checkLatestConfigFileRevision(uint8_t const _node);
    bool checkLatestMFSRevision();
    bool downloadLatestConfigFileRevision(uint8_t const _node);
    bool downloadLatestMFSRevision();


public slots:
    /* these slots are called from our OZWNotification Class. Applications should not call them */
    void pvt_valueAdded(uint64_t vidKey);
    void pvt_valueRemoved(uint64_t vidKey);
    void pvt_valueChanged(uint64_t vidKey);
    void pvt_valueRefreshed(uint64_t vidKey);
    void pvt_valuePollingEnabled(uint64_t vidKey);
    void pvt_valuePollingDisabled(uint64_t vidKey);
    void pvt_nodeGroupChanged(uint8_t node, uint8_t group);
    void pvt_nodeNew(uint8_t node);
    void pvt_nodeAdded(uint8_t node);
    void pvt_nodeRemoved(uint8_t node);
    void pvt_nodeReset(uint8_t node);
    void pvt_nodeNaming(uint8_t node);
    void pvt_nodeEvent(uint8_t node, uint8_t event);
    void pvt_nodeProtocolInfo(uint8_t node);
    void pvt_nodeEssentialNodeQueriesComplete(uint8_t node);
    void pvt_nodeQueriesComplete(uint8_t node);
    void pvt_driverReady(uint32_t homeID);
    void pvt_driverFailed(uint32_t homeID);
    void pvt_driverReset(uint32_t homeID);
    void pvt_driverRemoved(uint32_t homeID);
    void pvt_driverAllNodesQueriedSomeDead();
    void pvt_driverAllNodesQueried();
    void pvt_driverAwakeNodesQueried();
    void pvt_controllerCommand(uint8_t command);
    void pvt_ozwNotification(OpenZWave::Notification::NotificationCode event);
    void pvt_ozwUserAlert(OpenZWave::Notification::UserAlertNotification event);
    void pvt_manufacturerSpecificDBReady();

    void pvt_nodeModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void pvt_valueModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

private:
    bool Lock();
    bool Unlock();

private:

    bool checkHomeId();
    bool checkNodeId(uint8_t _node);
    bool convertValueID(uint64_t vidKey);


    OpenZWave::Options *m_options;
    OpenZWave::Manager *m_manager;
    QTOZW_Nodes_internal *m_nodeModel;
    QTOZW_ValueIds_internal *m_valueModel;
    pthread_mutex_t m_manager_mutex;
    QVector<uint8_t> m_validNodes;
    QVector<uint64_t> m_validValues;

};

#endif // QTOZWMANAGER_H
