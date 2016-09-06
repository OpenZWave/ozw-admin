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
#include <QInputDialog>
#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nodes.h"
#include "logwindow.h"
#include "devicedb.hpp"

#include "Options.h"
#include "Manager.h"
#include "Driver.h"
#include "Node.h"
#include "Group.h"
#include "Notification.h"
#include "value_classes/ValueStore.h"
#include "value_classes/Value.h"
#include "value_classes/ValueBool.h"
#include "platform/Log.h"
#include "Defs.h"


static pthread_mutex_t g_criticalSection;
static pthread_mutex_t initMutex = PTHREAD_MUTEX_INITIALIZER;

NodeList *ozwNodes;
int m_homeid;

//-----------------------------------------------------------------------------
// <OnNotification>
// Callback that is triggered when a value, group or node changes
//-----------------------------------------------------------------------------
void OnNotification
(
        OpenZWave::Notification const* _notification,
        void* _context
        )
{
    Q_UNUSED(_context);
    // Must do this inside a critical section to avoid conflicts with the main thread
    pthread_mutex_lock( &g_criticalSection );

    qDebug() << QString(_notification->GetAsString().c_str());
    switch( _notification->GetType() )
    {
    case OpenZWave::Notification::Type_ValueAdded:
    {
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
        ozwNodes->updateGroups(_notification->GetNodeId());
        break;
    }

    case OpenZWave::Notification::Type_NodeAdded:
    {
        Node *newNode = new Node(_notification->GetNodeId(), _notification->GetHomeId());
        ozwNodes->addNode(newNode);
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
        m_homeid = _notification->GetHomeId();
        break;
    }

    case OpenZWave::Notification::Type_DriverFailed:
    {
        break;
    }

    case OpenZWave::Notification::Type_AwakeNodesQueried:
    case OpenZWave::Notification::Type_AllNodesQueried:
    case OpenZWave::Notification::Type_AllNodesQueriedSomeDead:
    {
        OpenZWave::Manager::Get()->WriteConfig(_notification->GetHomeId());
        break;
    }

    case OpenZWave::Notification::Type_DriverReset:
    case OpenZWave::Notification::Type_Notification:
    {
        break;
    }
    case OpenZWave::Notification::Type_NodeNaming:
    {
        QModelIndex i = ozwNodes->getNodeValueIndex(_notification->GetNodeId(), NCN_NodeName);
        if (i.isValid())
            ozwNodes->setData(i,QVariant(OpenZWave::Manager::Get()->GetNodeName(_notification->GetHomeId(),_notification->GetNodeId()).c_str()));

    }
    case OpenZWave::Notification::Type_NodeProtocolInfo:
    case OpenZWave::Notification::Type_NodeQueriesComplete:
    default:
    {
    }
    }
    ozwNodes->updateQueryStage(_notification->GetNodeId());
    pthread_mutex_unlock( &g_criticalSection );
}





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);

    this->logBrowser = new LogBrowser;
    connect(ui->actionOpen_Log_Window, SIGNAL(triggered()), this, SLOT(openLogWindow()));

    connect(ui->actionOpen_Serial_Port, SIGNAL(triggered()), this, SLOT(OpenSerialPort()));
    connect(ui->action_Save_Cache, SIGNAL(triggered()), this, SLOT(saveCache()));
    connect(ui->actionDevice_Database, SIGNAL(triggered()), this, SLOT(OpenDeviceDB()));


    ozwNodes = new NodeList();
    connect(ozwNodes, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(resizeColumns()));

    this->ui->nodeList->setModel(ozwNodes);
    this->ui->nodeList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->nodeList->resizeColumnsToContents();
    this->ui->nodeList->horizontalHeader()->setStretchLastSection(true);
    this->ui->nodeList->verticalHeader()->hide();
    this->ui->nodeList->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->nodeList->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(NodeSelected(QModelIndex,QModelIndex)));


    SetCBReadOnly(this->ui->ni_beaming, true);
    SetCBReadOnly(this->ui->ni_flirs, true);
    SetCBReadOnly(this->ui->ni_listen,true);
    SetCBReadOnly(this->ui->ni_routing, true);
    SetCBReadOnly(this->ui->ni_security, true);
    SetCBReadOnly(this->ui->ni_zwplus, true);


    this->m_serialport = settings.value("SerialPort", "/dev/ttyUSB0").toString();

    pthread_mutexattr_t mutexattr;

    pthread_mutexattr_init ( &mutexattr );
    pthread_mutexattr_settype( &mutexattr, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init( &g_criticalSection, &mutexattr );
    pthread_mutexattr_destroy( &mutexattr );

    pthread_mutex_lock( &initMutex );


    printf("Starting OZWAdmin with OpenZWave Version %s\n", OpenZWave::Manager::getVersionAsString().c_str());




    OpenZWave::Options::Create( "../../../config/", "", "" );
    OpenZWave::Options::Get()->AddOptionInt( "SaveLogLevel", OpenZWave::LogLevel_Detail );
    OpenZWave::Options::Get()->AddOptionInt( "QueueLogLevel", OpenZWave::LogLevel_Debug );
    OpenZWave::Options::Get()->AddOptionInt( "DumpTrigger", OpenZWave::LogLevel_Error );
    OpenZWave::Options::Get()->AddOptionInt( "PollInterval", 500 );
    OpenZWave::Options::Get()->AddOptionBool( "IntervalBetweenPolls", true );
    OpenZWave::Options::Get()->AddOptionBool( "ValidateValueChanges", true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenSerialPort() {

    bool ok;
    QString text = QInputDialog::getText(this, tr("Select Serial Port"),
                                         tr("Serial Port:"), QLineEdit::Normal,
                                         this->m_serialport, &ok);
    if (!ok)
        return;

    /* check the port exists */
    QFileInfo check_port(text);
    if (!check_port.exists() && !check_port.isWritable()) {
        QMessageBox::warning(this, tr("OZW-Admin"),
                             tr("The Serial Port does not exist or is not accessable"),
                             QMessageBox::Ok);
        return;
    }

    this->m_serialport = text;
    this->settings.setValue("SerialPort", this->m_serialport);

    OpenZWave::Options::Get()->Lock();
    OpenZWave::Manager::Create();
    OpenZWave::Manager::Get()->AddWatcher( OnNotification, NULL );
    OpenZWave::Manager::Get()->AddDriver( this->m_serialport.toStdString());
    this->ui->action_Save_Cache->setEnabled(true);
}


void MainWindow::newNode(qint8 nodeID) {
    Q_UNUSED(nodeID);

}

void MainWindow::saveCache() {
    qDebug() << "Saving Cache for HomeID" << m_homeid;
    OpenZWave::Manager::Get()->WriteConfig(m_homeid);
}

void MainWindow::resizeColumns() {
    this->ui->nodeList->resizeColumnsToContents();
}

void MainWindow::NodeSelected(QModelIndex current,QModelIndex previous) {
    Q_UNUSED(previous);
    if (!current.isValid()) {
        return;
    }
    const QAbstractItemModel * model = current.model();
    qint8 nodeid = model->data(model->index(current.row(), 0), Qt::DisplayRole).toInt();
    Node *node = ozwNodes->getNode(nodeid);
    if (!node) {
        qDebug() << "Invalid Node";
        return;
    }
    this->ui->ni_zwplus->setChecked(node->getIsZWPlus());
    this->ui->NI_Manufacturer->setText(node->getNodeManufacturer());
    this->ui->NI_Product->setText(node->getNodeProduct());
    QString id(node->getNodeProductID());
    id.append(":").append(node->getNodeProductType());
    this->ui->NI_ID->setText(id);
    this->ui->ni_beaming->setChecked(node->getIsBeaming());
    this->ui->ni_flirs->setChecked(node->getIsFLiRS());
    this->ui->ni_listen->setChecked(node->getIsListening());
    this->ui->ni_routing->setChecked(node->getIsRouting());
    this->ui->ni_security->setChecked(node->getIsSecurity());
    this->ui->ni_version->setText(node->getNodeZWVersion());
    QString baud("Baud Rate: ");
    baud.append(node->getNodeBaudRate());
    this->ui->ni_baud->setText(baud);


    /* metaData info */
    this->ui->md_ozwinfo->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_OzwInfoPage));
    this->ui->md_pepper1db->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_Pepper1Page));
    this->ui->md_prodmanual->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_ProductManual));
    this->ui->md_prodpage->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_ProductPage));
    this->ui->md_zwprodpage->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_ZWProductPage));
//    QImage prodpic;
//    prodpic.load(node->getNodeMetaData(OpenZWave::Node::MetaData_ProductPic));
//    this->ui->md_prodimg->setPixmap(QPixmap::fromImage(prodpic));
    this->ui->md_prodimg->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_ProductPic));



    /* node status page */
    this->ui->ns_querystage->setText(node->getNodeQueryStage());
    this->ui->ns_sleeping->setChecked(!node->getIsNodeAwake());
    this->ui->ns_status->setText(node->getIsNodeFailed() ? "Failed" : "Ok");
    OpenZWave::Node::NodeData stats = node->getNodeStatistics();
    this->ui->ns_avgreqrtt->setText(QString::number(stats.m_averageRequestRTT));
    this->ui->ns_avgresprtt->setText(QString::number(stats.m_averageResponseRTT));
    this->ui->ns_lastrecieved->setText(stats.m_receivedTS.c_str());
    this->ui->ns_lastresprtt->setText(QString::number(stats.m_lastResponseRTT));
    this->ui->ns_lastseen->setText(stats.m_receivedTS.c_str());
    this->ui->ns_lastsend->setText(stats.m_sentTS.c_str());
    this->ui->ns_lreqrtt->setText(QString::number(stats.m_lastRequestRTT));
    this->ui->ns_quality->setText(QString::number(stats.m_quality));
    this->ui->ns_rcvdduplicates->setText(QString::number(stats.m_receivedDups));
    this->ui->ns_recievedcnt->setText(QString::number(stats.m_receivedCnt));
    this->ui->ns_retries->setText(QString::number(stats.m_retries));
    this->ui->ns_sentcnt->setText(QString::number(stats.m_sentCnt));
    this->ui->ns_sentfailed->setText(QString::number(stats.m_sentFailed));
    this->ui->ns_unsolicited->setText(QString::number(stats.m_receivedUnsolicited));



    this->ui->a_maxgroups->setText(QString::number(node->getNumGroups()));
    for (int i = 1; i <= node->getNumGroups(); i++)
        this->updateGroups(nodeid, i);

}


void MainWindow::openLogWindow() {
    this->logBrowser->show();
}

void MainWindow::updateGroups(qint8 nodeID, qint8 groupID) {
    Node *node = ozwNodes->getNode(nodeID);
    if (!node) {
        qDebug() << "Invalid Node";
        return;
    }
    associationinfo *group = node->getGroup(groupID);
    qDebug() << group->getGroupName();

}

void MainWindow::OpenDeviceDB() {
    DeviceDB *ddb = new DeviceDB();
    ddb->show();
}

