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
#include <QFileDialog>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nodes.h"
#include "logwindow.h"
#include "devicedb.hpp"
#include "valueid.h"
#include "valueiddelegate.h"

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
        QtValueID *vid = new QtValueID(_notification->GetValueID());
        ozwNodes->ValueAdded(vid);
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
        //settings.setvalue("SerialPort",
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

    this->m_VFproxyModel_System = new ValueIdFilterProxyModel(this);
    this->m_VFproxyModel_System->setFilterType(VF_Genre_System);
    this->m_VFproxyModel_User = new ValueIdFilterProxyModel(this);
    this->m_VFproxyModel_User->setFilterType(VF_Genre_User);
    this->m_VFproxyModel_Config = new ValueIdFilterProxyModel(this);
    this->m_VFproxyModel_Config->setFilterType(VF_Genre_Config);
    this->ui->val_config_tbl->setItemDelegateForColumn(2, new ValueIDDelegate());
    this->ui->val_config_tbl->setModel(this->m_VFproxyModel_Config);
    this->ui->val_system_tbl->setItemDelegateForColumn(2, new ValueIDDelegate());
    this->ui->val_system_tbl->setModel(this->m_VFproxyModel_System);
    this->ui->val_user_tbl->setItemDelegateForColumn(2, new ValueIDDelegate());
    this->ui->val_user_tbl->setModel(this->m_VFproxyModel_User);





    ozwNodes = new NodeList();
    connect(ozwNodes, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(resizeColumns()));

    this->ui->nodeList->setModel(ozwNodes);
    this->ui->nodeList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->nodeList->resizeColumnsToContents();
    this->ui->nodeList->horizontalHeader()->setStretchLastSection(true);
    this->ui->nodeList->verticalHeader()->hide();
    this->ui->nodeList->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->nodeList->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(NodeSelected(QModelIndex,QModelIndex)));

    this->ui->val_user_tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->val_user_tbl->resizeColumnsToContents();
    this->ui->val_user_tbl->horizontalHeader()->setStretchLastSection(true);
    this->ui->val_user_tbl->verticalHeader()->hide();
    this->ui->val_user_tbl->setSelectionMode(QAbstractItemView::SingleSelection);

    this->ui->val_config_tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->val_config_tbl->resizeColumnsToContents();
    this->ui->val_config_tbl->resizeRowsToContents();
    this->ui->val_config_tbl->horizontalHeader()->setStretchLastSection(true);
    this->ui->val_config_tbl->verticalHeader()->hide();
    this->ui->val_config_tbl->setSelectionMode(QAbstractItemView::SingleSelection);

    this->ui->val_system_tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->val_system_tbl->resizeColumnsToContents();
    this->ui->val_system_tbl->horizontalHeader()->setStretchLastSection(true);
    this->ui->val_system_tbl->verticalHeader()->hide();
    this->ui->val_system_tbl->setSelectionMode(QAbstractItemView::SingleSelection);

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

    m_configpath = settings.value("ConfigPath", "../../../config/").toString();
    m_userpath = settings.value("UserPath", "").toString();
    while (!m_configpath.exists()) {
        int ret = QMessageBox::critical(nullptr, "Select Device Database Path",
                                        QString("Please Select a Path to the Device Database"),
                                        QMessageBox::Open | QMessageBox::Abort);
        if (ret == QMessageBox::Open) {
            QString dir;
            dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    );
            QDir directory(dir);
            if (directory.exists("manufacturer_specific.xml")) {
                m_configpath.setPath(dir);
                settings.setValue("ConfigPath", dir);
            } else {
                if (QMessageBox::critical(nullptr, "Invalid Directory",
                                      QString("The Directory ").append(dir).append(" a Valid Device Database"),
                                          QMessageBox::Ok | QMessageBox::Abort) == QMessageBox::Abort) {
                    exit(-1);
                }
            }
        } else if (ret == QMessageBox::Abort) {
            exit(-1);
        }
    }
    OpenZWave::Options::Create( m_configpath.absolutePath().toStdString(), "", "" );
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
    OpenZWave::Manager::Get()->AddWatcher( OnNotification, this );
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
#if 0
    this->ui->md_ozwinfo->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_OzwInfoPage));
    this->ui->md_prodmanual->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_ProductManual));
    this->ui->md_prodpage->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_ProductPage));
    this->ui->md_zwprodpage->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_ZWProductPage));
    //    QImage prodpic;
    //    prodpic.load(node->getNodeMetaData(OpenZWave::Node::MetaData_ProductPic));
    //    this->ui->md_prodimg->setPixmap(QPixmap::fromImage(prodpic));
    this->ui->md_prodimg->setText(node->getNodeMetaData(OpenZWave::Node::MetaData_ProductPic));
#endif


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


    while (QLayoutItem *item = this->ui->groupsa->layout()->takeAt(1)) {
        QWidget *w = item->widget();

        this->ui->groupsa->layout()->removeWidget(w);
        w->deleteLater();
    }



    this->ui->a_maxgroups->setText(QString::number(node->getNumGroups()));
    for (int i = 1; i <= node->getNumGroups(); i++)
        this->updateGroups(nodeid, i);
    this->m_VFproxyModel_Config->setSourceModel(node->getValues());
    this->m_VFproxyModel_System->setSourceModel(node->getValues());
    this->m_VFproxyModel_User->setSourceModel(node->getValues());
    for (int i = 0; i < this->m_VFproxyModel_Config->rowCount(); ++i)
        this->ui->val_config_tbl->openPersistentEditor(this->m_VFproxyModel_Config->index(i, 2));
    this->ui->val_config_tbl->resizeColumnsToContents();
    this->ui->val_config_tbl->resizeRowsToContents();


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
    QGroupBox *agi = new QGroupBox("Group " + QString::number(group->getGroupID()) + ": " + group->getGroupName());
    agi->setStyleSheet("QGroupBox {border: 1px solid gray; border-radius: 9px;margin-top: 0.5em;} QGroupBox::title {subcontrol-origin: margin; left: 10px; padding: 0 3px 0 3px;}");
    QFormLayout *agifl = new QFormLayout();
    agi->setLayout(agifl);

    QLabel *malbl = new QLabel("Max Associations");
    QLabel *mav = new QLabel(QString::number(group->getMaxAssociations()));
    agifl->addRow(malbl, mav);



    this->ui->groupsa->layout()->addWidget(agi);
}

void MainWindow::OpenDeviceDB() {
    DeviceDB *ddb = new DeviceDB();
    ddb->show();
}

