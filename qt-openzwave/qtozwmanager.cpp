#include <unistd.h>
#include <QDebug>
#include "qtozwmanager.h"
#include "qtozwnodemodel.h"


#include "qtozwnotification.h"

#include "Manager.h"
#include "Options.h"
#include "Notification.h"
#include "platform/Log.h"

QString nodeBasicStr (uint8 basic);



void setupOZW() {




}


QTOZWManager::QTOZWManager()
{
    pthread_mutexattr_t mutexattr;

    pthread_mutexattr_init ( &mutexattr );
    pthread_mutexattr_settype( &mutexattr, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init( &m_manager_mutex, &mutexattr );
    pthread_mutexattr_destroy( &mutexattr );

    try {
        this->m_options = OpenZWave::Options::Create( "", "", "" );
    } catch (OpenZWave::OZWException &e) {
        qDebug() << "Failed to Load Options Class" << QString(e.GetMsg().c_str());
        return;
    }
    this->m_options->AddOptionInt( "SaveLogLevel", OpenZWave::LogLevel_Detail );
    this->m_options->AddOptionInt( "QueueLogLevel", OpenZWave::LogLevel_Debug );
    this->m_options->AddOptionInt( "DumpTrigger", OpenZWave::LogLevel_Error );
    this->m_options->AddOptionBool("ConsoleOutput", false);
    this->m_options->AddOptionInt( "PollInterval", 500 );
    this->m_options->AddOptionBool( "IntervalBetweenPolls", true );
    this->m_options->AddOptionBool( "ValidateValueChanges", true);

    this->m_nodeModel = new QTOZW_Nodes_internal(this);

}

bool QTOZWManager::Start(QString SerialPort)
{
    OpenZWave::Options::Get()->Lock();
    try {
        this->m_manager = OpenZWave::Manager::Create();
    } catch (OpenZWave::OZWException &e) {
        qDebug() << "Failed to Load " << QString(e.GetMsg().c_str());
        return false;
    }
    if (this->m_manager->AddWatcher( OZWNotification::processNotification, this ) != true) {
        qDebug() << "Failed to Add Notification Callback";
        return false;
    }

    /* setup all our Connections to the Notifications
    ** these are set as QueuedConnections as the Notifications can happen on a different thread
    ** to our main QT thread thats running this manager
    */
    QObject::connect(OZWNotification::Get(), &OZWNotification::valueAdded, this, &QTOZWManager::pvt_valueAdded, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::valueRemoved, this, &QTOZWManager::pvt_valueRemoved, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::valueChanged, this, &QTOZWManager::pvt_valueChanged, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::valueRefreshed, this, &QTOZWManager::pvt_valueRefreshed, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::valuePollingEnabled, this, &QTOZWManager::pvt_valuePollingEnabled, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::valuePollingDisabled, this, &QTOZWManager::pvt_valuePollingDisabled, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeGroupChanged, this, &QTOZWManager::pvt_nodeGroupChanged, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeNew, this, &QTOZWManager::pvt_nodeNew, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeAdded, this, &QTOZWManager::pvt_nodeAdded, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeRemoved, this, &QTOZWManager::pvt_nodeRemoved, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeReset, this, &QTOZWManager::pvt_nodeReset, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeNaming, this, &QTOZWManager::pvt_nodeNaming, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeEvent, this, &QTOZWManager::pvt_nodeEvent, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeProtocolInfo, this, &QTOZWManager::pvt_nodeProtocolInfo, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeEssentialNodeQueriesComplete, this, &QTOZWManager::pvt_nodeEssentialNodeQueriesComplete, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::nodeQueriesComplete, this, &QTOZWManager::pvt_nodeQueriesComplete, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::driverReady, this, &QTOZWManager::pvt_driverReady, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::driverFailed, this, &QTOZWManager::pvt_driverFailed, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::driverReset, this, &QTOZWManager::pvt_driverReset, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::driverRemoved, this, &QTOZWManager::pvt_driverRemoved, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::driverAllNodesQueriedSomeDead, this, &QTOZWManager::pvt_driverAllNodesQueriedSomeDead, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::driverAwakeNodesQueried, this, &QTOZWManager::pvt_driverAwakeNodesQueried, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::driverAllNodesQueried, this, &QTOZWManager::pvt_driverAllNodesQueried, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::controllerCommand, this, &QTOZWManager::pvt_controllerCommand, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::ozwNotification, this, &QTOZWManager::pvt_ozwNotification, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::ozwUserAlert, this, &QTOZWManager::pvt_ozwUserAlert, Qt::QueuedConnection);
    QObject::connect(OZWNotification::Get(), &OZWNotification::manufacturerSpecificDBReady, this, &QTOZWManager::pvt_manufacturerSpecificDBReady, Qt::QueuedConnection);



    if (this->m_manager->AddDriver( SerialPort.toStdString()) != true) {
        qDebug() << "Failed to Add Serial Port";
        return false;
    }
    qDebug() << "Started OZW";
  QThread::sleep(1);
  this->setRunning(false);
   QThread::sleep(1);
   this->setRunning(true);
   QThread::sleep(1);
   return true;
}

bool QTOZWManager::Lock() {
    if (pthread_mutex_lock( &this->m_manager_mutex ) == 0) {
        return true;
    }
    qDebug() << "Failed to Lock Manager Mutex";
    return false;
}
bool QTOZWManager::Unlock()
{
    if (pthread_mutex_unlock( &this->m_manager_mutex ) == 0) {
        return true;
    }
    qDebug() << "Failed to Unlock Manager Mutex";
    return false;

};

QTOZW_Nodes *QTOZWManager::getNodeModel() {
    return static_cast<QTOZW_Nodes *>(this->m_nodeModel);
}



void QTOZWManager::pvt_valueAdded(QTValueID vid)
{
    qDebug() << "Notification pvt_valueAdded";
    Q_UNUSED(vid);
}
void QTOZWManager::pvt_valueRemoved(QTValueID vid)
{
    qDebug() << "Notification pvt_valueRemoved";
    Q_UNUSED(vid);
}
void QTOZWManager::pvt_valueChanged(QTValueID vid)
{
    qDebug() << "Notification pvt_valueChanged";
    Q_UNUSED(vid);
}
void QTOZWManager::pvt_valueRefreshed(QTValueID vid)
{
    qDebug() << "Notification pvt_valueRefreshed";
    Q_UNUSED(vid);

}
void QTOZWManager::pvt_valuePollingEnabled(QTValueID vid)
{
    qDebug() << "Notification pvt_valuePollingEnabled";
    Q_UNUSED(vid);

}
void QTOZWManager::pvt_valuePollingDisabled(QTValueID vid)
{
    qDebug() << "Notification pvt_valuePollingDisabled";
    Q_UNUSED(vid);

}
void QTOZWManager::pvt_nodeGroupChanged(uint8_t node, uint8_t group)
{
    qDebug() << "Notification pvt_nodeGroupChanged";
    Q_UNUSED(node);
    Q_UNUSED(group);
}
void QTOZWManager::pvt_nodeNew(uint8_t node)
{
    qDebug() << "Notification pvt_nodeNew";
    this->m_nodeModel->addNode(node);

    QVariant data = this->m_manager->GetNodeQueryStage(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->m_homeID, node));


    emit this->nodeNew(node);
}
void QTOZWManager::pvt_nodeAdded(uint8_t node)
{
    qDebug() << "Notification pvt_nodeAdded";
    this->m_nodeModel->addNode(node);

    QVariant data = this->m_manager->GetNodeQueryStage(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->m_homeID, node));

    emit this->nodeAdded(node);

}
void QTOZWManager::pvt_nodeRemoved(uint8_t node)
{
    qDebug() << "Notification pvt_nodeRemoved";
    emit this->nodeRemoved(node);

}
void QTOZWManager::pvt_nodeReset(uint8_t node)
{
    qDebug() << "Notification pvt_nodeReset";
    emit this->nodeReset(node);

}
void QTOZWManager::pvt_nodeNaming(uint8_t node)
{
    qDebug() << "Notification pvt_nodeNaming";
    QString data = this->m_manager->GetNodeName(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeName, data);
    data = this->m_manager->GetNodeLocation(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeLocation, data);

    data = this->m_manager->GetNodeQueryStage(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->m_homeID, node));

    emit this->nodeNaming(node);
}
void QTOZWManager::pvt_nodeEvent(uint8_t node, uint8_t event)
{
    qDebug() << "Notification pvt_nodeEvent";

    QVariant data = this->m_manager->GetNodeQueryStage(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->m_homeID, node));

    emit this->nodeEvent(node, event);
}
void QTOZWManager::pvt_nodeProtocolInfo(uint8_t node)
{
    qDebug() << "Notification pvt_nodeProtocolInfo";

    QVariant data = this->m_manager->GetNodeProductName(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductName, data);

    data = this->m_manager->GetNodeManufacturerName(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeManufacturerName, data);

    data = this->m_manager->GetNodeManufacturerId(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeManufacturerID, data);

    data = this->m_manager->GetNodeProductId(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductID, data);

    data = this->m_manager->GetNodeProductType(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductType, data);

    data = this->m_manager->GetNodeProductId(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductID, data);

    data = this->m_manager->GetNodeProductType(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductType, data);

    data = nodeBasicStr(this->m_manager->GetNodeBasic(this->m_homeID, node));
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeBasicString, data);

    data = this->m_manager->GetNodeBasic(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeBasic, data);

    data = this->m_manager->GetNodeDeviceTypeString(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeGenericString, data);

    data = this->m_manager->GetNodeGeneric(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeGeneric, data);

    data = this->m_manager->GetNodeSpecific(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeSpecific, data);

    /* XXX TODO: Need a OZW SpecificString function */
    data = this->m_manager->GetNodeSpecific(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeSpecificString, data);

    data = this->m_manager->GetNodeMaxBaudRate(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeBaudRate, data);

    data = this->m_manager->GetNodeVersion(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeVersion, data);

    data = this->m_manager->GetNodeQueryStage(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

    /* set our Flags */
    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isListening, this->m_manager->IsNodeListeningDevice(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFlirs, this->m_manager->IsNodeFrequentListeningDevice(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isBeaming, this->m_manager->IsNodeBeamingDevice(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isRouting, this->m_manager->IsNodeRoutingDevice(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->m_homeID, node));

    emit this->nodeProtocolInfo(node);
}
void QTOZWManager::pvt_nodeEssentialNodeQueriesComplete(uint8_t node)
{
    qDebug() << "Notification pvt_nodeEssentialNodeQueriesComplete";
    QVariant data = this->m_manager->GetNodeQueryStage(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->m_homeID, node));

    emit this->nodeEssentialNodeQueriesComplete(node);
}
void QTOZWManager::pvt_nodeQueriesComplete(uint8_t node)
{
    qDebug() << "Notification pvt_nodeQueriesComplete";
    /* Plus Type Info here */
    QVariant data = this->m_manager->GetNodeDeviceType(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeDeviceType, data);

    data = this->m_manager->GetNodeDeviceTypeString(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeDeviceTypeString, data);

    data = this->m_manager->GetNodeRole(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeRole, data);

    data = this->m_manager->GetNodeRoleString(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeRoleString, data);

    data = this->m_manager->GetNodePlusType(this->m_homeID, node);
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodePlusType, data);

    data = this->m_manager->GetNodePlusTypeString(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodePlusTypeString, data);

    data = this->m_manager->GetNodeQueryStage(this->m_homeID, node).c_str();
    this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

    /* set our Flags */
    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isZWavePlus, this->m_manager->IsNodeZWavePlus(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->m_homeID, node));

    this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->m_homeID, node));

    emit this->nodeQueriesComplete(node);
}
void QTOZWManager::pvt_driverReady(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverRead";
    this->m_homeID = _homeID;
    emit this->driverReady(_homeID);
}
void QTOZWManager::pvt_driverFailed(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverFailed";
    emit this->driverFailed(_homeID);
}
void QTOZWManager::pvt_driverReset(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverReset";
    emit this->driverReset(_homeID);
}
void QTOZWManager::pvt_driverRemoved(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverRemoved";
    emit this->driverRemoved(_homeID);
}
void QTOZWManager::pvt_driverAllNodesQueriedSomeDead()
{
    qDebug() << "Notification pvt_driverAllNodesQueriedSomeDead";
    emit this->driverAllNodesQueriedSomeDead();
}
void QTOZWManager::pvt_driverAllNodesQueried()
{
    qDebug() << "Notification pvt_driverAllNodesQueried";
    emit this->driverAllNodesQueried();
}
void QTOZWManager::pvt_driverAwakeNodesQueried()
{
    qDebug() << "Notification pvt_driverAwakeNodesQueried";
    emit this->driverAllNodesQueried();
}
void QTOZWManager::pvt_controllerCommand(uint8_t command)
{
    qDebug() << "Notification pvt_controllerCommand";
    emit this->controllerCommand(command);
}
void QTOZWManager::pvt_ozwNotification(OpenZWave::Notification::NotificationCode event)
{
    qDebug() << "Notification pvt_ozwNotification";
    Q_UNUSED(event);

}
void QTOZWManager::pvt_ozwUserAlert(OpenZWave::Notification::UserAlertNotification event)
{
    qDebug() << "Notification pvt_ozwUserAlert";
    Q_UNUSED(event);

}
void QTOZWManager::pvt_manufacturerSpecificDBReady()
{
    qDebug() << "Notification pvt_manufacturerSpecificDBReady";
    emit this->manufacturerSpecificDBReady();
}



/* XXX TODO: this needs to go into OZW */
QString nodeBasicStr (uint8 basic)
{
  switch (basic) {
  case 1:
    return "Controller";
  case 2:
    return "Static Controller";
  case 3:
    return "Slave";
  case 4:
    return "Routing Slave";
  }
  return "unknown";
}

