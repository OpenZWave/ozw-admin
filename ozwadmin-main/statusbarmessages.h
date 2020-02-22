#ifndef STATUSBARMESSAGES_H
#define STATUSBARMESSAGES_H

#include <QObject>
#include <qt-openzwave/qtozwmanager.h>
#include <qt-openzwave/websocketiodevice.h>


class statusBarMessages : public QObject
{
    Q_OBJECT
public:
    explicit statusBarMessages(QObject *parent = nullptr);
    void setQTOZWManager(QTOZWManager *m_qtozwmanager);
signals:

public slots:
    void ready();
    void valueAdded(quint64 vidKey);
    void valueRemoved(quint64 vidKey);
    void valueChanged(quint64 vidKey);
    void valueRefreshed(quint64 vidKey);
    void nodeNew(quint8 node);
    void nodeAdded(quint8 node);
    void nodeRemoved(quint8 node);
    void nodeReset(quint8 node);
    void nodeNaming(quint8 node);
    void nodeEvent(quint8 node, quint8 event);
    void nodeProtocolInfo(quint8 node);
    void nodeEssentialNodeQueriesComplete(quint8 node);
    void nodeQueriesComplete(quint8 node);
    void nodeGroupChanged(quint8 node, quint8 group);
    void driverReady(quint32 homeID);
    void driverFailed(quint32 homeID);
    void driverReset(quint32 homeID);
    void driverRemoved(quint32 homeID);
    void driverAllNodesQueriedSomeDead();
    void driverAllNodesQueried();
    void driverAwakeNodesQueried();
    void controllerCommand(quint8 node, NotificationTypes::QTOZW_Notification_Controller_Cmd command, NotificationTypes::QTOZW_Notification_Controller_State state, NotificationTypes::QTOZW_Notification_Controller_Error error);
    void ozwNotification(quint8 node, NotificationTypes::QTOZW_Notification_Code event);
    void ozwUserAlert(quint8 node, NotificationTypes::QTOZW_Notification_User event, quint8 retry);
    void manufacturerSpecificDBReady();
    void starting();
    void started(quint32 homeID);
    void stopped(quint32 homeID);
//    void remoteConnectionStatus(WebSocketIoDevice::connectionStatus status, QAbstractSocket::SocketError error);


private:
    void setMessage(QString);

};

#endif // STATUSBARMESSAGES_H
