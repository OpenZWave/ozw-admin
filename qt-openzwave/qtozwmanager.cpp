#include <unistd.h>
#include <QDebug>
#include "qtozwmanager.h"



#include "Manager.h"
#include "Options.h"
#include "Notification.h"
#include "platform/Log.h"

void setupOZW() {
    qRegisterMetaType<QList<OZWNodes *>>("QList<OZWNodes *>");
}



//-----------------------------------------------------------------------------
// <OZWNotification>
// Callback that is triggered when a value, group or node changes
//-----------------------------------------------------------------------------
void OZWNotification
(
        OpenZWave::Notification const* _notification,
        void* _context
)
{
    QTOZWManager *manager = static_cast<QTOZWManager *>(_context);
    Q_UNUSED(manager);
    //Must do this inside a critical section to avoid conflicts with the main thread
    manager->Lock();

    qDebug() << QString(_notification->GetAsString().c_str());
    switch( _notification->GetType() )
    {
    case OpenZWave::Notification::Type_ValueAdded:
    {
//        QtValueID *vid = new QtValueID(_notification->GetValueID());
//        ozwNodes->ValueAdded(vid);
        break;
    }

    case OpenZWave::Notification::Type_ValueRemoved:
    {
        break;
    }

    case OpenZWave::Notification::Type_ValueChanged:
    {
        break;
    }

    case OpenZWave::Notification::Type_Group:
    {
//        ozwNodes->updateGroups(_notification->GetNodeId());
        break;
    }

    case OpenZWave::Notification::Type_NodeAdded:
    {
//        Node *newNode = new Node(_notification->GetNodeId(), _notification->GetHomeId());
//        ozwNodes->addNode(newNode);
        break;
    }

    case OpenZWave::Notification::Type_NodeRemoved:
    {
        break;
    }

    case OpenZWave::Notification::Type_NodeEvent:
    {
        break;
    }

    case OpenZWave::Notification::Type_PollingDisabled:
    {
        break;
    }

    case OpenZWave::Notification::Type_PollingEnabled:
    {
        break;
    }

    case OpenZWave::Notification::Type_DriverReady:
    {
//        m_homeid = _notification->GetHomeId();
        //settings.setvalue("SerialPort",
        break;
    }

    case OpenZWave::Notification::Type_DriverFailed:
    {
        manager->setRunning(false);
        break;
    }

    case OpenZWave::Notification::Type_AwakeNodesQueried:
    case OpenZWave::Notification::Type_AllNodesQueried:
    case OpenZWave::Notification::Type_AllNodesQueriedSomeDead:
    {
//        OpenZWave::Manager::Get()->WriteConfig(_notification->GetHomeId());
        break;
    }

    case OpenZWave::Notification::Type_DriverReset:
    case OpenZWave::Notification::Type_Notification:
    {
        break;
    }
    case OpenZWave::Notification::Type_NodeNaming:
    {
//        QModelIndex i = ozwNodes->getNodeValueIndex(_notification->GetNodeId(), NCN_NodeName);
//        if (i.isValid())
//            ozwNodes->setData(i,QVariant(OpenZWave::Manager::Get()->GetNodeName(_notification->GetHomeId(),_notification->GetNodeId()).c_str()));

    }
    case OpenZWave::Notification::Type_NodeProtocolInfo:
    case OpenZWave::Notification::Type_NodeQueriesComplete:
    default:
    {
    }
    }
//    ozwNodes->updateQueryStage(_notification->GetNodeId());
    manager->Unlock();
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
    this->m_options->AddOptionInt( "PollInterval", 500 );
    this->m_options->AddOptionBool( "IntervalBetweenPolls", true );
    this->m_options->AddOptionBool( "ValidateValueChanges", true);
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
    if (this->m_manager->AddWatcher( OZWNotification, this ) != true) {
        qDebug() << "Failed to Add Notification Callback";
        return false;
    }
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
