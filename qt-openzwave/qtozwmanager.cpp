#include <unistd.h>
#include <QDebug>
#include "qtozwmanager.h"
#include "qtozwnodemodel.h"
#include "qtozwassociations.h"


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
    QObject::connect(this->m_nodeModel, &QTOZW_Nodes_internal::dataChanged, this, &QTOZWManager::pvt_nodeModelDataChanged);
    this->m_valueModel = new QTOZW_ValueIds_internal(this);
    QObject::connect(this->m_valueModel, &QTOZW_ValueIds_internal::dataChanged, this, &QTOZWManager::pvt_valueModelDataChanged);
    this->m_associationsModel = new QTOZW_Associations_internal(this);

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

QTOZW_ValueIds *QTOZWManager::getValueModel() {
    return static_cast<QTOZW_ValueIds *>(this->m_valueModel);
}

QTOZW_Associations *QTOZWManager::getAssociationModel() {
    return static_cast<QTOZW_Associations *>(this->m_associationsModel);
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

bool QTOZWManager::checkValueKey(uint64_t _vidKey) {
    if (!this->m_validValues.contains(_vidKey)) {
        emit this->error(QTOZWErrorCodes::valueIDKey_Invalid);
        this->setErrorString("Invalid ValueID Key");
        return false;
    }
    return true;
}

bool QTOZWManager::convertValueID(uint64_t vidKey) {
    OpenZWave::ValueID vid(this->homeId(), vidKey);
    switch (vid.GetType()) {
        case OpenZWave::ValueID::ValueType_Bool:
        {
            bool value;
            this->m_manager->GetValueAsBool(vid, &value);
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Value, QVariant::fromValue(value));
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::Bool);
            return true;
        }
        case OpenZWave::ValueID::ValueType_Byte:
        {
            uint8_t value;
            this->m_manager->GetValueAsByte(vid, &value);
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Value, QVariant::fromValue(value));
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::Byte);
            return true;
        }
        case OpenZWave::ValueID::ValueType_Decimal:
        {
            std::string value;
            this->m_manager->GetValueAsString(vid, &value);
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Value, QVariant::fromValue(QString(value.c_str()).toFloat()));
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::Decimal);
            return true;
        }
        case OpenZWave::ValueID::ValueType_Int:
        {
            int32_t value;
            this->m_manager->GetValueAsInt(vid, &value);
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Value, QVariant::fromValue(value));
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::Int);
            return true;
        }
        case OpenZWave::ValueID::ValueType_List:
        {
            std::vector<std::string> items;
            std::vector<int32> values;
            this->m_manager->GetValueListItems(vid, &items);
            this->m_manager->GetValueListValues(vid, &values);
            if (items.size() != values.size()) {
                qWarning() << "ValueList Item Size Does not equal Value Size";
            } else {
                std::vector<std::string>::iterator it;
                size_t i = 0;
                QTOZW_ValueIDList vidlist;
                for(it = items.begin(); it != items.end(); it++) {
                    vidlist.labels.push_back(QString::fromStdString(*it));
                    vidlist.values.push_back(values.at(i));
                    i++;
                }
                std::string selectedItem;
                this->m_manager->GetValueListSelection(vid, &selectedItem);
                vidlist.selectedItem = QString::fromStdString(selectedItem);
                this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Value, QVariant::fromValue(vidlist));
            }
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::List);
            return true;
        }
        case OpenZWave::ValueID::ValueType_Schedule:
        {
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::Schedule);
            return true;
        }
        case OpenZWave::ValueID::ValueType_Short:
        {
            int16_t value;
            this->m_manager->GetValueAsShort(vid, &value);
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Value, QVariant::fromValue(value));
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::Short);
            return true;
        }
        case OpenZWave::ValueID::ValueType_String:
        {
            std::string value;
            this->m_manager->GetValueAsString(vid, &value);
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Value, QVariant::fromValue(QString(value.c_str())));
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::String);
            return true;
        }
        case OpenZWave::ValueID::ValueType_Button:
        {
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::Button);
            return true;
        }
        case OpenZWave::ValueID::ValueType_Raw:
        {
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::Raw);
            return true;
        }
        case OpenZWave::ValueID::ValueType_BitSet:
        {
            uint8_t bssize;
            int32_t bsmask;
            this->m_manager->GetBitSetSize(vid, &bssize);
            this->m_manager->GetBitMask(vid, &bsmask);
            QTOZW_ValueIDBitSet vidbs;
            vidbs.mask.resize(INT32_MAX);
            for (int i = 0; i < 32; ++i) {
                vidbs.mask[i] = bsmask & (1 << i);
            }
            vidbs.values.resize(bssize * 8);
            qDebug() << vidbs.values.size();
            for (uint8_t i = 0; i < 32; ++i) {
                bool value;
                this->m_manager->GetValueAsBitSet(vid, i, &value);
                vidbs.values[i] = value;
                vidbs.label[i] = QString::fromStdString(this->m_manager->GetValueLabel(vid, i));
                vidbs.help[i] = QString::fromStdString(this->m_manager->GetValueHelp(vid, i));
            }
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Value, QVariant::fromValue(vidbs));
            this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Type, QTOZW_ValueIds::ValueIdTypes::BitSet);
            return true;
        }
    }
    return false;
}


void QTOZWManager::pvt_valueAdded(uint64_t vidKey)
{
    qDebug() << "Notification pvt_valueAdded";
    if (!this->m_validValues.contains(vidKey))
        this->m_validValues.push_back(vidKey);

    this->m_valueModel->addValue(vidKey);

    try {
        OpenZWave::ValueID vid(this->homeId(), vidKey);
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Label, this->m_manager->GetValueLabel(vid).c_str());
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Instance, vid.GetInstance());
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::CommandClass, vid.GetCommandClassId());
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Index, vid.GetIndex());
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Node, vid.GetNodeId());
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Genre, vid.GetGenre());
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Help, this->m_manager->GetValueHelp(vid).c_str());
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Units, this->m_manager->GetValueUnits(vid).c_str());
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Min, this->m_manager->GetValueMin(vid));
        this->m_valueModel->setValueData(vidKey, QTOZW_ValueIds::ValueIdColumns::Max, this->m_manager->GetValueMax(vid));
        this->m_valueModel->setValueFlags(vidKey, QTOZW_ValueIds::ValueIDFlags::ReadOnly, this->m_manager->IsValueReadOnly(vid));
        this->m_valueModel->setValueFlags(vidKey, QTOZW_ValueIds::ValueIDFlags::WriteOnly, this->m_manager->IsValueWriteOnly(vid));
        this->m_valueModel->setValueFlags(vidKey, QTOZW_ValueIds::ValueIDFlags::ValueSet, this->m_manager->IsValueSet(vid));
        this->m_valueModel->setValueFlags(vidKey, QTOZW_ValueIds::ValueIDFlags::ValuePolled, this->m_manager->IsValuePolled(vid));
        this->m_valueModel->setValueFlags(vidKey, QTOZW_ValueIds::ValueIDFlags::ChangeVerified, this->m_manager->GetChangeVerified(vid));

        this->convertValueID(vidKey);
    } catch (OpenZWave::OZWException &e) {
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
    emit this->valueAdded(vidKey);
}
void QTOZWManager::pvt_valueRemoved(uint64_t vidKey)
{
    qDebug() << "Notification pvt_valueRemoved";
    if (this->m_validValues.contains(vidKey))
        this->m_validValues.removeAll(vidKey);
    this->m_valueModel->delValue(vidKey);
    emit this->valueRemoved(vidKey);
    Q_UNUSED(vidKey);
}
void QTOZWManager::pvt_valueChanged(uint64_t vidKey)
{
    qDebug() << "Notification pvt_valueChanged";
    this->convertValueID(vidKey);
    emit this->valueChanged(vidKey);
}
void QTOZWManager::pvt_valueRefreshed(uint64_t vidKey)
{
    qDebug() << "Notification pvt_valueRefreshed";

    this->convertValueID(vidKey);
    emit this->valueRefreshed(vidKey);
}
void QTOZWManager::pvt_valuePollingEnabled(uint64_t vidKey)
{
    qDebug() << "Notification pvt_valuePollingEnabled";
    this->m_valueModel->setValueFlags(vidKey, QTOZW_ValueIds::ValueIDFlags::ValuePolled, true);

}
void QTOZWManager::pvt_valuePollingDisabled(uint64_t vidKey)
{
    qDebug() << "Notification pvt_valuePollingDisabled";
    this->m_valueModel->setValueFlags(vidKey, QTOZW_ValueIds::ValueIDFlags::ValuePolled, false);

}
void QTOZWManager::pvt_nodeGroupChanged(uint8_t node, uint8_t group)
{
    qDebug() << "Notification pvt_nodeGroupChanged";

    if (this->m_associationDefaultsSet[node][group] == false) {
        for (int i = 0; i < this->m_manager->GetNumGroups(this->homeId(), node); i++) {
            this->m_associationsModel->setDefaultGroupData(node, group, QTOZW_Associations::associationColumns::MaxAssocations, this->m_manager->GetMaxAssociations(this->homeId(), node, group));
            this->m_associationsModel->setDefaultGroupFlags(node, group, QTOZW_Associations::associationFlags::isMultiInstance, this->m_manager->IsMultiInstance(this->homeId(), node, group));
            this->m_associationsModel->setDefaultGroupData(node, group, QTOZW_Associations::associationColumns::GroupName, this->m_manager->GetGroupLabel(this->homeId(), node, group).c_str());
        }
        this->m_associationDefaultsSet[node][group] = true;
    }
    OpenZWave::InstanceAssociation *ia;
    uint32_t count = this->m_manager->GetAssociations(this->homeId(), node, group, &ia);
    for (uint32_t i = 0;  i < count; i++) {
        if (!this->m_associationsModel->isAssociationExists(node, group, ia[i].m_nodeId, ia[i].m_instance)) {
            this->m_associationsModel->addAssociation(node, group, ia[i].m_nodeId, ia[i].m_instance);
        }
    }
    /* now see if we need to delete any entries for this node/group */
    for (int row = 0; row < this->m_associationsModel->rowCount(QModelIndex()); ++row) {
        QMap<QTOZW_Associations::associationColumns, QVariant> data = this->m_associationsModel->m_associationData[row];
        bool found = false;
        for (uint32_t i = 0; i < count; i++) {
            if (data[QTOZW_Associations::associationColumns::NodeID] == node) {
                if (data[QTOZW_Associations::associationColumns::GroupID] == group) {
                    if (data[QTOZW_Associations::associationColumns::MemberNodeID] == ia[i].m_nodeId) {
                        if (data[QTOZW_Associations::associationColumns::MemberNodeInstance] == ia[i].m_instance) {
                            found = true;
                        }
                    }
                }
            }
        }
        if (found == false) {
            this->m_associationsModel->delAssociation(node, group, data[QTOZW_Associations::associationColumns::MemberNodeID].value<uint8_t>(), data[QTOZW_Associations::associationColumns::MemberNodeInstance].value<uint8_t>());
        }
    }
    if (ia != nullptr)
        delete [] ia;
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
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
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
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
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
    this->m_associationsModel->delNode(node);
    /* technically, this shouldn't be required... but just in case */
    this->m_valueModel->delNodeValues(node);
    this->m_nodeModel->delNode(node);
    emit this->nodeRemoved(node);

}
void QTOZWManager::pvt_nodeReset(uint8_t node)
{
    qDebug() << "Notification pvt_nodeReset";
    if (this->m_validNodes.contains(node))
        this->m_validNodes.removeAll(node);

    this->m_associationsModel->delNode(node);
    /* technically, this shouldn't be required... but just in case */
    this->m_valueModel->delNodeValues(node);
    this->m_nodeModel->delNode(node);
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
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
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
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
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
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
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

        this->m_nodeModel->setNodeData(node, QTOZW_Nodes::NodeGroups, this->m_manager->GetNumGroups(this->homeId(), node));
    } catch (OpenZWave::OZWException &e) {
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
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
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
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
    this->m_associationsModel->resetModel();
    this->m_valueModel->resetModel();
    this->m_nodeModel->resetModel();
    emit this->stopped(this->homeId());
    emit this->driverFailed(_homeID);
    this->setHomeId(0);
}
void QTOZWManager::pvt_driverReset(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverReset";
    this->m_associationsModel->resetModel();
    this->m_valueModel->resetModel();
    this->m_nodeModel->resetModel();
    emit this->stopped(this->homeId());
    emit this->driverReset(_homeID);
    this->setHomeId(0);
}
void QTOZWManager::pvt_driverRemoved(uint32_t _homeID)
{
    qDebug() << "Notification pvt_driverRemoved";
    this->m_associationsModel->resetModel();
    this->m_valueModel->resetModel();
    this->m_nodeModel->resetModel();
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

void QTOZWManager::pvt_nodeModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(bottomRight);
    if (!roles.contains(QTOZW_UserRoles::ModelDataChanged)) {
        return;
    }
    qDebug() << "nodeModel Changed!" << static_cast<QTOZW_Nodes::NodeColumns>(topLeft.column()) << ": "<< topLeft.data();
    /* get the Node Number */
    QModelIndex nodeIdIndex = topLeft.siblingAtColumn(QTOZW_Nodes::NodeColumns::NodeID);
    uint8_t nodeId = this->m_nodeModel->data(nodeIdIndex, Qt::DisplayRole).value<uint8_t>();
    try {
        switch (static_cast<QTOZW_Nodes::NodeColumns>(topLeft.column())) {
            case QTOZW_Nodes::NodeColumns::NodeName:
                this->m_manager->SetNodeName(this->homeId(), nodeId, topLeft.data().toString().toStdString());
                break;
            case QTOZW_Nodes::NodeColumns::NodeLocation:
                this->m_manager->SetNodeLocation(this->homeId(), nodeId, topLeft.data().toString().toStdString());
                break;
            default:
                qWarning() << "Got a nodeModelDataChanged Singal but not a Column we handle: " << static_cast<QTOZW_Nodes::NodeColumns>(topLeft.column()) ;
                return;
        }
    } catch (OpenZWave::OZWException &e) {
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }

}

void QTOZWManager::pvt_valueModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
    Q_UNUSED(bottomRight);
    if (!roles.contains(QTOZW_UserRoles::ModelDataChanged)) {
        return;
    }
    qDebug() << "valueModel Changed!" << static_cast<QTOZW_ValueIds::ValueIdColumns>(topLeft.column()) << ": "<< topLeft.data();
    /* get the Node Number */
    uint64_t vidKey = this->m_valueModel->data(topLeft.siblingAtColumn(QTOZW_ValueIds::ValueIdColumns::ValueIDKey), Qt::DisplayRole).value<uint64_t>();
    try {
        OpenZWave::ValueID vid(this->homeId(), vidKey);
        switch (static_cast<OpenZWave::ValueID::ValueType>(vid.GetType())) {
            case OpenZWave::ValueID::ValueType_Bool:
            {
                this->m_manager->SetValue(vid, topLeft.data().toBool());
                return;
            }
            case OpenZWave::ValueID::ValueType_Byte:
            {
                this->m_manager->SetValue(vid, topLeft.data().value<uint8_t>());
                return;
            }
            case OpenZWave::ValueID::ValueType_Decimal:
            {
                this->m_manager->SetValue(vid, topLeft.data().toFloat());
                return;
            }
            case OpenZWave::ValueID::ValueType_Int:
            {
                this->m_manager->SetValue(vid, topLeft.data().value<int32>());
                return;
            }
            case OpenZWave::ValueID::ValueType_List:
            {
                qWarning() << "ValueType List TODO";
                return;
            }
            case OpenZWave::ValueID::ValueType_Schedule:
            {
                qWarning() << "ValueType_Schedule TODO";
                return;
            }
            case OpenZWave::ValueID::ValueType_Short:
            {
                this->m_manager->SetValue(vid, topLeft.data().value<int16>());
                return;
            }
            case OpenZWave::ValueID::ValueType_String:
            {
                this->m_manager->SetValue(vid, topLeft.data().toString().toStdString());
                return;
            }
            case OpenZWave::ValueID::ValueType_Button:
            {
                qWarning() << "ValueType_Button TODO";
                return;
            }
            case OpenZWave::ValueID::ValueType_Raw:
            {
                qWarning() << "ValueType_Raw TODO";
                return;
            }
            case OpenZWave::ValueID::ValueType_BitSet:
            {
                qWarning() << "ValueType_BitSet TODO";
                return;
            }

        }
    } catch (OpenZWave::OZWException &e) {
        qWarning() << "OZW Exception: " << e.GetMsg().c_str() << " at " << e.GetFile().c_str() <<":" << e.GetLine();
        emit this->error(QTOZWErrorCodes::OZWException);
        this->setErrorString(e.GetMsg().c_str());
    }
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

