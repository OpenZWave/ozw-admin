#include <QMainWindow>
#include <QStatusBar>
#include "statusbarmessages.h"
#include "ozwcore.h"

statusBarMessages::statusBarMessages(QObject *parent) : QObject(parent)
{
    QTOZWManager *qtozwm = OZWCore::get()->getQTOZWManager();
    QObject::connect(qtozwm, &QTOZWManager::manufacturerSpecificDBReady, this, &statusBarMessages::manufacturerSpecificDBReady);
    QObject::connect(qtozwm, &QTOZWManager::ready, this, &statusBarMessages::ready);
    QObject::connect(qtozwm, &QTOZWManager::starting, this, &statusBarMessages::starting);
    QObject::connect(qtozwm, &QTOZWManager::started, this, &statusBarMessages::started);
    QObject::connect(qtozwm, &QTOZWManager::valueAdded, this, &statusBarMessages::valueAdded);
    QObject::connect(qtozwm, &QTOZWManager::valueRemoved, this, &statusBarMessages::valueRemoved);
    QObject::connect(qtozwm, &QTOZWManager::valueChanged, this, &statusBarMessages::valueChanged);
    QObject::connect(qtozwm, &QTOZWManager::valueRefreshed, this, &statusBarMessages::valueRefreshed);
    QObject::connect(qtozwm, &QTOZWManager::nodeNew, this, &statusBarMessages::nodeNew);
    QObject::connect(qtozwm, &QTOZWManager::nodeAdded, this, &statusBarMessages::nodeAdded);
    QObject::connect(qtozwm, &QTOZWManager::nodeRemoved, this, &statusBarMessages::nodeRemoved);
    QObject::connect(qtozwm, &QTOZWManager::nodeReset, this, &statusBarMessages::nodeReset);
    QObject::connect(qtozwm, &QTOZWManager::nodeNaming, this, &statusBarMessages::nodeNaming);
    QObject::connect(qtozwm, &QTOZWManager::nodeEvent, this, &statusBarMessages::nodeEvent);
    QObject::connect(qtozwm, &QTOZWManager::nodeProtocolInfo, this, &statusBarMessages::nodeProtocolInfo);
    QObject::connect(qtozwm, &QTOZWManager::nodeEssentialNodeQueriesComplete, this, &statusBarMessages::nodeEssentialNodeQueriesComplete);
    QObject::connect(qtozwm, &QTOZWManager::nodeQueriesComplete, this, &statusBarMessages::nodeQueriesComplete);
    QObject::connect(qtozwm, &QTOZWManager::nodeGroupChanged, this, &statusBarMessages::nodeGroupChanged);
    QObject::connect(qtozwm, &QTOZWManager::driverReady, this, &statusBarMessages::driverReady);
    QObject::connect(qtozwm, &QTOZWManager::driverFailed, this, &statusBarMessages::driverFailed);
    QObject::connect(qtozwm, &QTOZWManager::driverReset, this, &statusBarMessages::driverReady);
    QObject::connect(qtozwm, &QTOZWManager::driverRemoved, this, &statusBarMessages::driverRemoved);
    QObject::connect(qtozwm, &QTOZWManager::driverAllNodesQueriedSomeDead, this, &statusBarMessages::driverAllNodesQueriedSomeDead);
    QObject::connect(qtozwm, &QTOZWManager::driverAllNodesQueried, this, &statusBarMessages::driverAllNodesQueried);
    QObject::connect(qtozwm, &QTOZWManager::driverAwakeNodesQueried, this, &statusBarMessages::driverAwakeNodesQueried);
    QObject::connect(qtozwm, &QTOZWManager::controllerCommand, this, &statusBarMessages::controllerCommand);
    QObject::connect(qtozwm, &QTOZWManager::ozwNotification, this, &statusBarMessages::ozwNotification);
//    QObject::connect(qtozwm, &QTOZWManager::remoteConnectionStatus, this, &statusBarMessages::remoteConnectionStatus);
    QObject::connect(qtozwm, &QTOZWManager::ozwUserAlert, this, &statusBarMessages::ozwUserAlert);
    QObject::connect(qtozwm, &QTOZWManager::manufacturerSpecificDBReady, this, &statusBarMessages::manufacturerSpecificDBReady);
}

void statusBarMessages::ready() {
    emit newMessage("OpenZwave Ready");
}
void statusBarMessages::valueAdded(quint64 vidKey) {
    emit newMessage(QString("Value Added: %1").arg(vidKeyDetails(vidKey)));
}
void statusBarMessages::valueRemoved(quint64 vidKey) {
    emit newMessage(QString("Value Removed: %1").arg(vidKeyDetails(vidKey)));
}
void statusBarMessages::valueChanged(quint64 vidKey) {
    emit newMessage(QString("Value Changed: %1").arg(vidKeyDetails(vidKey)));
}
void statusBarMessages::valueRefreshed(quint64 vidKey) {
    emit newMessage(QString("Value Refreshed: %1").arg(vidKeyDetails(vidKey)));
}
void statusBarMessages::nodeNew(quint8 node) {
    emit newMessage(QString("New Node: %1").arg(node));
}
void statusBarMessages::nodeAdded(quint8 node) {
    emit newMessage(QString("Node Added: %1").arg(node));
}
void statusBarMessages::nodeRemoved(quint8 node) {
    emit newMessage(QString("Node Removed: %1").arg(node));
}
void statusBarMessages::nodeReset(quint8 node) {
    emit newMessage(QString("Node Reset: %1").arg(node));
}
void statusBarMessages::nodeNaming(quint8 node) {
    emit newMessage(QString("Node Name Received: %1").arg(node));
}
void statusBarMessages::nodeEvent(quint8 node, quint8 event) {
    emit newMessage(QString("Node %1 Basic Event Received: %2").arg(node).arg(event));
}
void statusBarMessages::nodeProtocolInfo(quint8 node) {
    emit newMessage(QString("Node Basic Protocol Information Received: %2").arg(node));
}
void statusBarMessages::nodeEssentialNodeQueriesComplete(quint8 node) {
    emit newMessage(QString("Essential Node Queries Complete: %2").arg(node));
}
void statusBarMessages::nodeQueriesComplete(quint8 node) {
    emit newMessage(QString("Node Queries Complete: %2").arg(node));
}
void statusBarMessages::nodeGroupChanged(quint8 node, quint8 group) {
    emit newMessage(QString("Node %1 Group Update Received: %2").arg(node).arg(group));
}
void statusBarMessages::driverReady(quint32 homeID) {
    emit newMessage(QString("Driver Ready for HomeID: %2").arg(homeID));
}
void statusBarMessages::driverFailed(quint32 homeID) {
    emit newMessage(QString("Driver Failed for HomeID: %2").arg(homeID));
}
void statusBarMessages::driverReset(quint32 homeID) {
    emit newMessage(QString("Driver Reset for HomeID: %2").arg(homeID));
}
void statusBarMessages::driverRemoved(quint32 homeID) {
    emit newMessage(QString("Driver Removed for HomeID: %2").arg(homeID));
}
void statusBarMessages::driverAllNodesQueriedSomeDead() {
    emit newMessage(QString("ZWave Network Node Queries Completed - Some Dead Nodes"));
}
void statusBarMessages::driverAllNodesQueried() {
    emit newMessage(QString("ZWave Network Node Queries Completed"));
}
void statusBarMessages::driverAwakeNodesQueried() {
    emit newMessage(QString("ZWave Network Awake Node Queries Completed"));
}
void statusBarMessages::controllerCommand(quint8 node, NotificationTypes::QTOZW_Notification_Controller_Cmd command, NotificationTypes::QTOZW_Notification_Controller_State state, NotificationTypes::QTOZW_Notification_Controller_Error error) {
    Q_UNUSED(node);
    QMetaEnum commandEnum = QMetaEnum::fromType<NotificationTypes::QTOZW_Notification_Controller_Cmd>();
    QMetaEnum stateEnum = QMetaEnum::fromType<NotificationTypes::QTOZW_Notification_Controller_State>();
    QMetaEnum errorEnum = QMetaEnum::fromType<NotificationTypes::QTOZW_Notification_Controller_Error>();
    emit newMessage(QString("ZWave Controller Command Executed: %1 State: %2 Error: %3").arg(commandEnum.valueToKey(command)).arg(stateEnum.valueToKey(state)).arg(errorEnum.valueToKey(error)));
}
void statusBarMessages::ozwNotification(quint8 node, NotificationTypes::QTOZW_Notification_Code event) {
    Q_UNUSED(node);
    QMetaEnum eventEnum = QMetaEnum::fromType<NotificationTypes::QTOZW_Notification_Code>();
    emit newMessage(QString("ZWave Network Notification Received: %1").arg(eventEnum.valueToKey(event)));
}
void statusBarMessages::ozwUserAlert(quint8 node, NotificationTypes::QTOZW_Notification_User event, quint8 retry) {
    Q_UNUSED(node);
    Q_UNUSED(retry);
    QMetaEnum eventEnum = QMetaEnum::fromType<NotificationTypes::QTOZW_Notification_User>();
    emit newMessage(QString("ZWave User Alert Received: %1").arg(eventEnum.valueToKey(event)));
}
void statusBarMessages::manufacturerSpecificDBReady() {
    emit newMessage(QString("Manufacturer Specific Database Loaded"));
}
void statusBarMessages::starting() {
    emit newMessage(QString("Starting Z-Wave Network"));
}
void statusBarMessages::started(quint32 homeID) {
    emit newMessage(QString("Started Z-Wave Network: %1").arg(homeID));
}
void statusBarMessages::stopped(quint32 homeID) {
    emit newMessage(QString("Stopped Z-Wave Network: %1").arg(homeID));
}
#if 0
void statusBarMessages::remoteConnectionStatus(connectionStatus status, QAbstractSocket::SocketError error) {

}
#endif

QString statusBarMessages::vidKeyDetails(quint64 key) {
    quint8 node = OZWCore::get()->getQTOZWManager()->getNodeId(key);
    quint8 instance = OZWCore::get()->getQTOZWManager()->getInstance(key);
    QString label = OZWCore::get()->getQTOZWManager()->getValueLabel(key);
    QString status(QString("Node %1, Instance %2, Label %3").arg(node).arg(instance).arg(label));
    return status;
}
