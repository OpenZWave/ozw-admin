#include <unistd.h>
#include <QDebug>
#include "qtozwmanager.h"
#include "qtozwnodemodel.h"


#include "qtozwnotification.h"

#include "Manager.h"
#include "Options.h"
#include "Notification.h"
#include "platform/Log.h"
#include "OZWException.h"

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
    emit this->starting();
    OpenZWave::Options::Get()->Lock();
    try {
        this->m_manager = OpenZWave::Manager::Create();
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
        qWarning() << "Failed to Load Manager Class" << QString(e.GetMsg().c_str());
        return false;
    }
    try {
        if (this->m_manager->AddWatcher( OZWNotification::processNotification, this ) != true) {
            emit this->error(QTOZWErrorCodes::setupFailed);
            this->setErrorString("Failed to Add Notification Callback");
            qWarning() << "Failed to Add Notification Callback";
            return false;
        }
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
        qWarning() << "Failed to Add Notification Callback " << QString(e.GetMsg().c_str());
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


    try {
        if (this->m_manager->AddDriver( SerialPort.toStdString()) != true) {
            emit this->error(QTOZWErrorCodes::setupFailed);
            this->setErrorString("Failed to Add Serial Port");
            qWarning() << "Failed to Add Serial Port";
            return false;
        }
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
        qWarning() << "Failed to Add Serial Port: " << QString(e.GetMsg().c_str());
        return false;
    }
    return true;
}

bool QTOZWManager::refreshNodeInfo(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->RefreshNodeInfo(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

bool QTOZWManager::requestNodeState(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->RequestNodeState(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

bool QTOZWManager::requestNodeDynamic(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->RequestNodeDynamic(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

bool QTOZWManager::setConfigParam(uint8_t _node, uint8_t _param, int32_t _value, uint8_t const _size) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->SetConfigParam(this->homeId(), _node, _param, _value, _size);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

void QTOZWManager::requestConfigParam(uint8_t _node, uint8_t _param) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return;
    try {
        this->m_manager->RequestConfigParam(this->homeId(), _node, _param);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return;
}

void QTOZWManager::requestAllConfigParam(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return;
    try {
        this->m_manager->RequestAllConfigParams(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return;
}

void QTOZWManager::softResetController() {
    if (!this->checkHomeId())
        return;
    try {
        this->m_manager->SoftReset(this->homeId());
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return;

}

void QTOZWManager::hardResetController() {
    if (!this->checkHomeId())
        return;
    try {
        this->m_manager->ResetController(this->homeId());
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return;

}

bool QTOZWManager::cancelControllerCommand() {
    if (!this->checkHomeId())
        return false;
    try {
        return this->m_manager->CancelControllerCommand(this->homeId());
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

void QTOZWManager::testNetworkNode(uint8_t _node, uint32_t const _count) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return;
    try {
        this->m_manager->TestNetworkNode(this->homeId(), _node, _count);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return;
}
void QTOZWManager::testNetwork(uint32_t const _count) {
    if (!this->checkHomeId() )
        return;
    try {
        this->m_manager->TestNetwork(this->homeId(), _count);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return;
}
void QTOZWManager::healNetworkNode(uint8_t _node, bool _doRR) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return;
    try {
        this->m_manager->HealNetworkNode(this->homeId(), _node, _doRR);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return;
}
void QTOZWManager::healNetwork(bool _doRR) {
    if (!this->checkHomeId())
        return;
    try {
        this->m_manager->HealNetwork(this->homeId(), _doRR);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return;
}
bool QTOZWManager::addNode(bool _doSecure) {
    if (!this->checkHomeId())
        return false;
    try {
        return this->m_manager->AddNode(this->homeId(), _doSecure);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::removeNode() {
    if (!this->checkHomeId())
        return false;
    try {
        return this->m_manager->RemoveNode(this->homeId());
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::removeFailedNode(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->RemoveFailedNode(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::hasNodeFailed(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->HasNodeFailed(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::requestNodeNeighborUpdate(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->RequestNodeNeighborUpdate(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::assignReturnRoute(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->AssignReturnRoute(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::deleteAllReturnRoute(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->DeleteAllReturnRoutes(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::sendNodeInfomation(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->SendNodeInformation(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::replaceFailedNode(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->ReplaceFailedNode(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}
bool QTOZWManager::requestNetworkUpdate(uint8_t _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->RequestNetworkUpdate(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

bool QTOZWManager::checkLatestConfigFileRevision(uint8_t const _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->checkLatestConfigFileRevision(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

bool QTOZWManager::checkLatestMFSRevision() {
    if (!this->checkHomeId())
        return false;
    try {
        return this->m_manager->checkLatestMFSRevision(this->homeId());
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

bool QTOZWManager::downloadLatestConfigFileRevision(uint8_t const _node) {
    if (!this->checkHomeId() || !this->checkNodeId(_node))
        return false;
    try {
        return this->m_manager->downloadLatestConfigFileRevision(this->homeId(), _node);
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
}

bool QTOZWManager::downloadLatestMFSRevision() {
    if (!this->checkHomeId())
        return false;
    try {
        return this->m_manager->downloadLatestMFSRevision(this->homeId());
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    return false;
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

bool QTOZWManager::checkHomeId() {
    if (this->m_manager == nullptr) {
        emit this->error(QTOZWErrorCodes::Manager_Not_Started);
        this->setErrorString("Manager Not Started");
        return false;
    }
    if (this->homeId() == 0) {
        emit this->error(QTOZWErrorCodes::homeId_Invalid);
        this->setErrorString("Invalid homeID");
        return false;
    }
    return true;
}
bool QTOZWManager::checkNodeId(uint8_t _node) {
    if (!this->m_validNodes.contains(_node)) {
        emit this->error(QTOZWErrorCodes::nodeId_Invalid);
        this->setErrorString("Invalid nodeID");
        return false;
    }
    return true;
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
    if (!this->m_validNodes.contains(node))
        this->m_validNodes.push_back(node);
    this->m_nodeModel->addNode(node);
    try {
        QVariant data = this->m_manager->GetNodeQueryStage(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->homeId(), node));
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }

    emit this->nodeNew(node);
}
void QTOZWManager::pvt_nodeAdded(uint8_t node)
{
    qDebug() << "Notification pvt_nodeAdded";
    if (!this->m_validNodes.contains(node))
        this->m_validNodes.push_back(node);

    this->m_nodeModel->addNode(node);
    try {
        QVariant data = this->m_manager->GetNodeQueryStage(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->homeId(), node));
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    emit this->nodeAdded(node);

}
void QTOZWManager::pvt_nodeRemoved(uint8_t node)
{
    qDebug() << "Notification pvt_nodeRemoved";
    if (this->m_validNodes.contains(node))
        this->m_validNodes.removeAll(node);

    emit this->nodeRemoved(node);

}
void QTOZWManager::pvt_nodeReset(uint8_t node)
{
    qDebug() << "Notification pvt_nodeReset";
    if (this->m_validNodes.contains(node))
        this->m_validNodes.removeAll(node);

    emit this->nodeReset(node);

}
void QTOZWManager::pvt_nodeNaming(uint8_t node)
{
    qDebug() << "Notification pvt_nodeNaming";
    try {
        QString data = this->m_manager->GetNodeName(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeName, data);
        data = this->m_manager->GetNodeLocation(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeLocation, data);

        data = this->m_manager->GetNodeQueryStage(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->homeId(), node));
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    emit this->nodeNaming(node);
}
void QTOZWManager::pvt_nodeEvent(uint8_t node, uint8_t event)
{
    qDebug() << "Notification pvt_nodeEvent";
    try {
        QVariant data = this->m_manager->GetNodeQueryStage(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->homeId(), node));
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    emit this->nodeEvent(node, event);
}
void QTOZWManager::pvt_nodeProtocolInfo(uint8_t node)
{
    qDebug() << "Notification pvt_nodeProtocolInfo";
    try {
        QVariant data = this->m_manager->GetNodeProductName(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductName, data);

        data = this->m_manager->GetNodeManufacturerName(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeManufacturerName, data);

        data = this->m_manager->GetNodeManufacturerId(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeManufacturerID, data);

        data = this->m_manager->GetNodeProductId(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductID, data);

        data = this->m_manager->GetNodeProductType(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductType, data);

        data = this->m_manager->GetNodeProductId(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductID, data);

        data = this->m_manager->GetNodeProductType(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeProductType, data);

        data = nodeBasicStr(this->m_manager->GetNodeBasic(this->homeId(), node));
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeBasicString, data);

        data = this->m_manager->GetNodeBasic(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeBasic, data);

        data = this->m_manager->GetNodeDeviceTypeString(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeGenericString, data);

        data = this->m_manager->GetNodeGeneric(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeGeneric, data);

        data = this->m_manager->GetNodeSpecific(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeSpecific, data);

        /* XXX TODO: Need a OZW SpecificString function */
        data = this->m_manager->GetNodeSpecific(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeSpecificString, data);

        data = this->m_manager->GetNodeMaxBaudRate(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeBaudRate, data);

        data = this->m_manager->GetNodeVersion(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeVersion, data);

        data = this->m_manager->GetNodeQueryStage(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

        /* set our Flags */
        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isListening, this->m_manager->IsNodeListeningDevice(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFlirs, this->m_manager->IsNodeFrequentListeningDevice(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isBeaming, this->m_manager->IsNodeBeamingDevice(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isRouting, this->m_manager->IsNodeRoutingDevice(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->homeId(), node));
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    emit this->nodeProtocolInfo(node);
}
void QTOZWManager::pvt_nodeEssentialNodeQueriesComplete(uint8_t node)
{
    qDebug() << "Notification pvt_nodeEssentialNodeQueriesComplete";
    try {
        QVariant data = this->m_manager->GetNodeQueryStage(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->homeId(), node));
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    emit this->nodeEssentialNodeQueriesComplete(node);
}
void QTOZWManager::pvt_nodeQueriesComplete(uint8_t node)
{
    qDebug() << "Notification pvt_nodeQueriesComplete";
    /* Plus Type Info here */
    try {
        QVariant data = this->m_manager->GetNodeDeviceType(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeDeviceType, data);

        data = this->m_manager->GetNodeDeviceTypeString(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeDeviceTypeString, data);

        data = this->m_manager->GetNodeRole(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeRole, data);

        data = this->m_manager->GetNodeRoleString(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeRoleString, data);

        data = this->m_manager->GetNodePlusType(this->homeId(), node);
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodePlusType, data);

        data = this->m_manager->GetNodePlusTypeString(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodePlusTypeString, data);

        data = this->m_manager->GetNodeQueryStage(this->homeId(), node).c_str();
        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeQueryStage, data);

        /* set our Flags */
        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isZWavePlus, this->m_manager->IsNodeZWavePlus(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isNIFRecieved, this->m_manager->IsNodeInfoReceived(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isAwake, this->m_manager->IsNodeAwake(this->homeId(), node));

        this->m_nodeModel->setNodeFlags(node, QTOZW_Nodes::isFailed, this->m_manager->IsNodeFailed(this->homeId(), node));
    } catch (OpenZWave::OZWException &e) {
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    emit this->nodeQueriesComplete(node);
}
void QTOZWManager::pvt_driverReady(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverRead";
    this->setHomeId(_homeID);
    emit this->started(_homeID);
    emit this->driverReady(_homeID);
}
void QTOZWManager::pvt_driverFailed(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverFailed";
    emit this->stopped(this->homeId());
    emit this->driverFailed(_homeID);
    this->setHomeId(0);
}
void QTOZWManager::pvt_driverReset(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverReset";
    emit this->stopped(this->homeId());
    emit this->driverReset(_homeID);
    this->setHomeId(0);
}
void QTOZWManager::pvt_driverRemoved(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverRemoved";
    emit this->stopped(this->homeId());
    emit this->driverRemoved(_homeID);
    this->setHomeId(0);
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

