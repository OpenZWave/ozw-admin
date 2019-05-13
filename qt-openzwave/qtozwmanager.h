#ifndef QTOZWMANAGER_H
#define QTOZWMANAGER_H

#include <QObject>
#include "rep_qtozwmanager_source.h"
#include "qtozwnotification.h"
#include "qtozwnodemodel.h"

void setupOZW();


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


public slots:
    /* these slots are called from our OZWNotification Class. Applications should not call them */
    void pvt_valueAdded(QTValueID);
    void pvt_valueRemoved(QTValueID);
    void pvt_valueChanged(QTValueID);
    void pvt_valueRefreshed(QTValueID);
    void pvt_valuePollingEnabled(QTValueID);
    void pvt_valuePollingDisabled(QTValueID);
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

private:
    bool Lock();
    bool Unlock();

private:
    OpenZWave::Options *m_options;
    OpenZWave::Manager *m_manager;
    QTOZW_Nodes_internal *m_nodeModel;
    pthread_mutex_t m_manager_mutex;
    uint32_t m_homeID;
};

#endif // QTOZWMANAGER_H
