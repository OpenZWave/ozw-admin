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
#include <QStandardPaths>

#include <qt-openzwave/qt-openzwavedatabase.h>
#include <qt-openzwave/qtozwproxymodels.h>
#include <qt-openzwave/qtozwoptions.h>
#include <qt-openzwave/qtozw_pods.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "metadatawindow.h"
#include "logwindow.h"
#include "devicedb.hpp"
#include "value_delegate.h"
#include "node_delegate.h"
#include "configuration.h"
#include "startup.h"
#include "startupprogress.h"
#include "util.h"


void SetReadOnly(QCheckBox* checkBox, bool readOnly)
{
   checkBox->setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
   checkBox->setFocusPolicy(readOnly ? Qt::NoFocus : Qt::StrongFocus);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->ui->action_Close->setEnabled(false);
    connect(ui->actionOpen_Log_Window, SIGNAL(triggered()), this, SLOT(openLogWindow()));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OpenConnection()));
    connect(ui->action_Close, SIGNAL(triggered()), this, SLOT(CloseConnection()));
    connect(ui->actionDevice_Database, SIGNAL(triggered()), this, SLOT(OpenDeviceDB()));
    connect(ui->md_helpwindow, &QPushButton::clicked, this, &MainWindow::openMetaDataWindow);
    connect(ui->action_Configuration, SIGNAL(triggered()), this, SLOT(openConfigWindow()));

    Node_Delegate *nodeflagdelegate = new Node_Delegate(this);

    this->ui->nodeList->setItemDelegateForColumn(QTOZW_Nodes::NodeColumns::NodeFlags, nodeflagdelegate);
    this->ui->nodeList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->nodeList->resizeColumnsToContents();
    this->ui->nodeList->verticalHeader()->hide();
    this->ui->nodeList->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->nodeList->setSortingEnabled(true);
    this->ui->nodeList->horizontalHeader()->setSectionsMovable(true);
//    this->ui->nodeList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    this->ui->nodeList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->val_user_tbl->resizeColumnsToContents();



    Value_Delegate *delegate = new Value_Delegate(this);

    this->ui->val_user_tbl->setItemDelegateForColumn(QTOZW_ValueIds::ValueIdColumns::Value, delegate);
    this->ui->val_user_tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->val_user_tbl->verticalHeader()->hide();
    this->ui->val_user_tbl->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->val_user_tbl->setSortingEnabled(true);
    this->ui->val_user_tbl->horizontalHeader()->setSectionsMovable(true);
//    this->ui->val_user_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    this->ui->val_user_tbl->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->val_user_tbl->resizeColumnsToContents();



    this->ui->val_config_tbl->setItemDelegateForColumn(QTOZW_ValueIds::ValueIdColumns::Value, delegate);
    this->ui->val_config_tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->val_config_tbl->horizontalHeader()->setStretchLastSection(true);
    this->ui->val_config_tbl->verticalHeader()->hide();
    this->ui->val_config_tbl->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->val_config_tbl->setSortingEnabled(true);
    this->ui->val_config_tbl->horizontalHeader()->setSectionsMovable(true);
//    this->ui->val_config_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    this->ui->val_config_tbl->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->val_config_tbl->resizeColumnsToContents();



    this->ui->val_system_tbl->setItemDelegateForColumn(QTOZW_ValueIds::ValueIdColumns::Value, delegate);
    this->ui->val_system_tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->val_system_tbl->verticalHeader()->hide();
    this->ui->val_system_tbl->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->val_system_tbl->setSortingEnabled(true);
    this->ui->val_system_tbl->horizontalHeader()->setSectionsMovable(true);
//    this->ui->val_system_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    this->ui->val_system_tbl->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->val_system_tbl->resizeColumnsToContents();



    SetReadOnly(this->ui->ni_flirs, true);
    SetReadOnly(this->ui->ni_listen, true);
    SetReadOnly(this->ui->ni_zwplus, true);
    SetReadOnly(this->ui->ni_beaming, true);
    SetReadOnly(this->ui->ni_routing, true);
    SetReadOnly(this->ui->ni_security, true);


    QStringList PossibleDBPaths;
    PossibleDBPaths << settings.value("openzwave/ConfigPath", QDir::toNativeSeparators("../../../config/")).toString().append("/");
    PossibleDBPaths << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);

    QString path, dbPath, userPath;
    foreach(path, PossibleDBPaths) {
        qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path+"/config/manufacturer_specific.xml")).absoluteFilePath() << " for manufacturer_specific.xml";
        if (QFileInfo(QDir::toNativeSeparators(path+"/config/manufacturer_specific.xml")).exists()) {
            dbPath = QFileInfo(QDir::toNativeSeparators(path+"/config/manufacturer_specific.xml")).absoluteFilePath();
            break;
        }
        qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path+"../config/manufacturer_specific.xml")).absoluteFilePath() << " for manufacturer_specific.xml";
        if (QFile(QDir::toNativeSeparators(path+"/../config/manufacturer_specific.xml")).exists()) {
            dbPath = QFileInfo(QDir::toNativeSeparators(path+"/../config/manufacturer_specific.xml")).absoluteFilePath();
            break;
        }
    }
    PossibleDBPaths.clear();
    PossibleDBPaths << settings.value("openzwave/UserPath", QDir::toNativeSeparators("../../../config/")).toString().append("/");
    PossibleDBPaths << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);

    foreach(path, PossibleDBPaths) {
        qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path+"/config/Options.xml")).absoluteFilePath() << " for Options.xml";
        if (QFileInfo(QDir::toNativeSeparators(path+"/config/Options.xml")).exists()) {
            userPath = QFileInfo(QDir::toNativeSeparators(path+"/config/Options.xml")).absoluteFilePath();
            break;
        }
        qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path+"/../config/Options.xml")).absoluteFilePath() << " for Options.xml";
        if (QFile(QDir::toNativeSeparators(path+"/../config/Options.xml")).exists()) {
            userPath = QFileInfo(QDir::toNativeSeparators(path+"/../config/Options.xml")).absoluteFilePath();
            break;
        }
    }

    qCDebug(ozwadmin) << "DBPath: " << dbPath;
    qCDebug(ozwadmin) << "userPath: " << userPath;

    m_configpath = settings.value("openzwave/ConfigPath", "../../../config/").toString().append("/");
    m_userpath = settings.value("openzwave/UserPath", "").toString().append("/");
    while (!m_configpath.exists()) {
        int ret = QMessageBox::critical(nullptr, tr("OpenZWave Database Path"),
                                        tr("The OpenZWave Database was not Found\n") +
                                        tr("Would you like to install a new copy, or use a existing database?"),
                                        QMessageBox::Open | QMessageBox::Save |     QMessageBox::Abort);
        if (ret == QMessageBox::Open) {
            QString dir;
            dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    );
            QDir directory(dir);
            if (directory.exists("manufacturer_specific.xml")) {
                m_configpath.setPath(dir);
                settings.setValue("openzwave/ConfigPath", dir.append("/"));
                settings.setValue("openzwave/UserPath", dir.append("/"));
            } else {
                if (QMessageBox::critical(nullptr, "Invalid Directory",
                                      QString("The Directory ").append(dir).append(" a Valid Device Database"),
                                          QMessageBox::Ok | QMessageBox::Abort) == QMessageBox::Abort) {
                    exit(-1);
                }
            }
        } else if (ret == QMessageBox::Save) {
            QString dir;
            dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    );
            QFileInfo directory(dir);
            qCDebug(ozwadmin) << directory.absoluteFilePath();
            if (directory.exists()) {
#ifndef _WIN32
                QStringList dirs;
                dirs << directory.absoluteFilePath().append("/");
                initConfigDatabase(dirs);
#if 0
                copyConfigDatabase(directory.absoluteFilePath().append("/"));
#endif
#endif
                m_configpath.setPath(directory.absoluteFilePath().append("/config/"));
                m_userpath.setPath(directory.absoluteFilePath().append("/config/"));
                settings.setValue("openzwave/ConfigPath", m_configpath.absolutePath());
                settings.setValue("openzwave/UserPath", m_userpath.absolutePath());

            }
        } else if (ret == QMessageBox::Abort) {
            exit(-1);
        }
    }
    this->m_openzwave = new QTOpenZwave(this, m_configpath, m_userpath);
    this->m_qtozwmanager = this->m_openzwave->GetManager();
    QObject::connect(this->m_qtozwmanager, &QTOZWManager::ready, this, &MainWindow::QTOZW_Ready);

    this->m_qtozwmanager->initilizeSource(this->settings.value("StartServer").toBool());
    this->m_logWindow.setModel(this->m_qtozwmanager->getLogModel());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::QTOZW_Ready() {
    qCDebug(ozwadmin) << "QTOZW Ready";

    /* apply our Local Configuration Options to the OZW Options Class */
    settings.beginGroup("openzwave");
    QStringList optionlist = settings.allKeys();
    for (int i = 0; i < optionlist.size(); i++) {
        qCDebug(ozwadmin) << "Updating Option " << optionlist.at(i) << " to " << settings.value(optionlist.at(i));
        QTOZWOptions *ozwoptions = this->m_qtozwmanager->getOptions();
        QStringList listtypes;
        listtypes << "SaveLogLevel" << "QueueLogLevel" << "DumpLogLevel";
        if (listtypes.contains(optionlist.at(i))) {
            OptionList list = ozwoptions->property(optionlist.at(i).toLocal8Bit()).value<OptionList>();
            if (list.getEnums().size() > 0)
                list.setSelected(settings.value(optionlist.at(i)).toString());
        }
        else
        {
            ozwoptions->setProperty(optionlist.at(i).toLocal8Bit(), settings.value(optionlist.at(i)));
        }
    }
    settings.endGroup();

    QTOZW_proxyNodeModel *nodeList = new QTOZW_proxyNodeModel(this);
    nodeList->setSourceModel(this->m_qtozwmanager->getNodeModel());
    this->ui->nodeList->setModel(nodeList);
    for (int i = 0; i <= QTOZW_Nodes::NodeColumns::NodeCount; i++) {
        switch (i) {
            case QTOZW_Nodes::NodeColumns::NodeID:
            case QTOZW_Nodes::NodeColumns::NodeName:
            case QTOZW_Nodes::NodeColumns::NodeProductName:
            case QTOZW_Nodes::NodeColumns::NodeFlags:
            break;
        default:
            this->ui->nodeList->horizontalHeader()->hideSection(i);
        }
    }
    QObject::connect(ui->nodeList->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::NodeSelected);

    QTOZW_proxyValueModel *userList = new QTOZW_proxyValueModel(this);
    userList->setSourceModel(this->m_qtozwmanager->getValueModel());
    userList->setFilterGenre(QTOZW_ValueIds::ValueIdGenres::User);
    userList->setSelectionModel(this->ui->nodeList->selectionModel());
    this->ui->val_user_tbl->setModel(userList);

    for (int i = 0; i <= QTOZW_ValueIds::ValueIdColumns::ValueIdCount; i++) {
        switch (i) {
            case QTOZW_ValueIds::ValueIdColumns::Label:
            case QTOZW_ValueIds::ValueIdColumns::Value:
            case QTOZW_ValueIds::ValueIdColumns::Instance:
            case QTOZW_ValueIds::ValueIdColumns::Help:
            break;
        default:
            this->ui->val_user_tbl->horizontalHeader()->hideSection(i);
        }
    }

    QTOZW_proxyValueModel *configList = new QTOZW_proxyValueModel(this);
    configList->setSourceModel(this->m_qtozwmanager->getValueModel());
    configList->setFilterGenre(QTOZW_ValueIds::ValueIdGenres::Config);
    configList->setSelectionModel(this->ui->nodeList->selectionModel());
    this->ui->val_config_tbl->setModel(configList);

    for (int i = 0; i <= QTOZW_ValueIds::ValueIdColumns::ValueIdCount; i++) {
        switch (i) {
            case QTOZW_ValueIds::ValueIdColumns::Label:
            case QTOZW_ValueIds::ValueIdColumns::Value:
            case QTOZW_ValueIds::ValueIdColumns::Help:
            break;
        default:
            this->ui->val_config_tbl->horizontalHeader()->hideSection(i);
        }
    }


    QTOZW_proxyValueModel *systemList = new QTOZW_proxyValueModel(this);
    systemList->setSourceModel(this->m_qtozwmanager->getValueModel());
    systemList->setFilterGenre(QTOZW_ValueIds::ValueIdGenres::System);
    systemList->setSelectionModel(this->ui->nodeList->selectionModel());
    this->ui->val_system_tbl->setModel(systemList);

    for (int i = 0; i <= QTOZW_ValueIds::ValueIdColumns::ValueIdCount; i++) {
        switch (i) {
            case QTOZW_ValueIds::ValueIdColumns::Label:
            case QTOZW_ValueIds::ValueIdColumns::Value:
            case QTOZW_ValueIds::ValueIdColumns::Instance:
            case QTOZW_ValueIds::ValueIdColumns::Help:
            break;
        default:
            this->ui->val_system_tbl->horizontalHeader()->hideSection(i);
        }
    }

    QObject::connect(&this->m_statTimer, &QTimer::timeout, this, &MainWindow::updateNodeStats);
    this->m_statTimer.start(1000);
}

void MainWindow::OpenConnection() {

    this->ui->actionOpen->setEnabled(false);
    this->ui->action_Close->setEnabled(true);

	Startup su(this);
	su.setModal(true);
	int ret = su.exec();
	if (ret == QDialog::Accepted) {

		if (su.getremote() == true) {
			qCDebug(ozwadmin) << "Doing Remote Connection:" << su.getremoteHost() << su.getremotePort();
			QUrl server;
			server.setHost(su.getremoteHost());
			server.setPort(su.getremotePort());
			server.setScheme("ws");
			qCDebug(ozwadmin) << "Connecting to " << server;
			startupprogress *sup = new startupprogress(true, this);
			sup->setQTOZWManager(this->m_qtozwmanager);
			sup->show();
            this->m_qtozwmanager->setClientAuth(su.getauthKey());
			this->m_qtozwmanager->initilizeReplica(server);
            this->settings.setValue("connection/remotehost", su.getremoteHost());
            this->settings.setValue("connection/remoteport", su.getremotePort());
            this->settings.setValue("connection/authKey", su.getauthKey());
			return;
		}
		else 
		{
			qCDebug(ozwadmin) << "Doing Local Connection: " << su.getserialPort() << su.getstartServer();
			this->m_serialport = su.getserialPort();
			startupprogress *sup = new startupprogress(false, this);
			sup->setQTOZWManager(this->m_qtozwmanager);
			sup->show();
			this->m_qtozwmanager->open(this->m_serialport);
            this->settings.setValue("connection/serialport", su.getserialPort());
            this->settings.setValue("connection/startserver", su.getstartServer());
			return;
		}
	}
	qCDebug(ozwadmin) << "Open Dialog was Canceled" << ret;

}
void MainWindow::CloseConnection() {

}



void MainWindow::resizeColumns() {
    this->ui->nodeList->resizeColumnsToContents();
}


#include "nodeflagswidget.h"

void MainWindow::NodeSelected(QModelIndex current,QModelIndex previous) {
    Q_UNUSED(previous);
    if (!current.isValid()) {
        return;
    }
    const QAbstractItemModel * model = current.model();

    /* I tried to use the QDataWidgetMapper but failed... */
    this->ui->dd_name->setText(model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeProductName)).toString());
    this->ui->dd_manufacturer->setText(model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeManufacturerName)).toString());
    this->ui->dd_manid->setText(model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeManufacturerID)).toString());
    this->ui->dd_prodid->setText(model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeProductID)).toString());
    this->ui->dd_prodtype->setText(model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeProductType)).toString());

    quint8 node = model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeID)).value<quint8>();

    QUrl url = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::OZWInfoURL);
    if (url.isValid())
        this->ui->md_ozwinfo->setText("<a href='"+url.toEncoded()+"'>"+tr("OpenZWave Database")+"</a>");
    else
        this->ui->md_ozwinfo->setText("");

    url = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ZWAProductURL);
    if (url.isValid())
        this->ui->md_zwainfo->setText("<a href='"+url.toEncoded()+"'>"+tr("ZWave Alliance Database")+"</a>");
    else
        this->ui->md_zwainfo->setText("");

    url = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ProductManualURL);
    if (url.isValid())
        this->ui->md_manual->setText("<a href='"+url.toEncoded()+"'>"+tr("Product Manual")+"</a>");
    else
        this->ui->md_manual->setText("");

    url = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ProductSupportURL);
    if (url.isValid())
        this->ui->md_prodpage->setText("<a href='"+url.toEncoded()+"'>"+tr("Product Support")+"</a>");
    else
        this->ui->md_prodpage->setText("");

    QString value = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::Name);
    this->ui->md_name->setText(value);

    value = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ProductPageURL);
    this->ui->md_producturl->setText(value);

    QPixmap pix;
    pix.loadFromData(this->m_qtozwmanager->GetMetaDataProductPic(model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeID)).value<quint8>()));
    this->ui->md_pic->setPixmap(pix);

    /* Now Do the Device Flags */
    QBitArray flags = model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeFlags)).value<QBitArray>();
    this->ui->ni_flirs->setChecked(flags.at(QTOZW_Nodes::nodeFlags::isFlirs));
    this->ui->ni_listen->setChecked(flags.at(QTOZW_Nodes::nodeFlags::isListening));
    this->ui->ni_zwplus->setChecked(flags.at(QTOZW_Nodes::nodeFlags::isZWavePlus));
    this->ui->ni_beaming->setChecked(flags.at(QTOZW_Nodes::nodeFlags::isBeaming));
    this->ui->ni_routing->setChecked(flags.at(QTOZW_Nodes::nodeFlags::isRouting));
    this->ui->ni_security->setChecked(flags.at(QTOZW_Nodes::nodeFlags::isSecurityv1));
    this->ui->ni_baud->setText(model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeBaudRate)).toString());

    /* now do the Node Status Page */
    updateNodeStats();

    //this->ui->ns_sleeping
}

void MainWindow::updateNodeStats() {
    QModelIndex current = this->ui->nodeList->currentIndex();
    if (!current.isValid())
        return;
    const QAbstractItemModel * model = current.model();
    quint8 node = model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeID)).value<quint8>();
    QBitArray flags = model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeFlags)).value<QBitArray>();
    this->ui->ns_querystage->setText(this->m_qtozwmanager->GetNodeQueryStage(node));
    this->ui->ns_sleeping->setChecked(!flags.at(QTOZW_Nodes::nodeFlags::isAwake));

    if (flags.at(QTOZW_Nodes::nodeFlags::isFailed)) {
        this->ui->ns_status->setText("Dead");
    } else if (!flags.at(QTOZW_Nodes::nodeFlags::isAwake)) {
        this->ui->ns_status->setText("Sleeping");
    } else {
        this->ui->ns_status->setText("Awake");
    }

    NodeStatistics ns = this->m_qtozwmanager->GetNodeStatistics(node);
    this->ui->ns_lastseen->setText(ns.lastReceivedTimeStamp.toString());
    this->ui->ns_lreqrtt->setText(QVariant::fromValue<quint32>(ns.lastRequestRTT).toString());
    this->ui->ns_quality->setText(QVariant::fromValue<quint32>(ns.quality).toString());
    this->ui->ns_retries->setText(QVariant::fromValue<quint32>(ns.retries).toString());
    this->ui->ns_sentcnt->setText(QVariant::fromValue<quint32>(ns.sentCount).toString());
    this->ui->ns_lastsend->setText(ns.lastSentTimeStamp.toString());
    this->ui->ns_avgreqrtt->setText(QVariant::fromValue<quint32>(ns.averageRequestRTT).toString());
    this->ui->ns_avgresprtt->setText(QVariant::fromValue<quint32>(ns.averageResponseRTT).toString());
    this->ui->ns_sentfailed->setText(QVariant::fromValue<quint32>(ns.sentFailed).toString());
    this->ui->ns_lastresprtt->setText(QVariant::fromValue<quint32>(ns.lastResponseRTT).toString());
    this->ui->ns_recievedcnt->setText(QVariant::fromValue<quint32>(ns.receivedPackets).toString());
    this->ui->ns_unsolicited->setText(QVariant::fromValue<quint32>(ns.receivedUnsolicited).toString());
    this->ui->ns_lastrecieved->setText(ns.lastReceivedTimeStamp.toString());
    this->ui->ns_rcvdduplicates->setText(QVariant::fromValue<quint32>(ns.receivedDupPackets).toString());

    this->ui->etxstatus_frame->setVisible(ns.extendedTXSupported);

    this->ui->ens_hops->setText(QVariant::fromValue<quint8>(ns.hops).toString());
    this->ui->ens_rssi_1->setText(ns.rssi_1);
    this->ui->ens_rssi_2->setText(ns.rssi_2);
    this->ui->ens_rssi_3->setText(ns.rssi_3);
    this->ui->ens_rssi_4->setText(ns.rssi_4);
    this->ui->ens_rssi_5->setText(ns.rssi_5);
    this->ui->ens_route_1->setText(QVariant::fromValue<quint8>(ns.route_1).toString());
    this->ui->ens_route_2->setText(QVariant::fromValue<quint8>(ns.route_2).toString());
    this->ui->ens_route_3->setText(QVariant::fromValue<quint8>(ns.route_3).toString());
    this->ui->ens_route_4->setText(QVariant::fromValue<quint8>(ns.route_4).toString());
    this->ui->ens_txtime->setText(QVariant::fromValue<quint16>(ns.txTime).toString());
    this->ui->ens_attempts->setText(QVariant::fromValue<quint8>(ns.routeTries).toString());
    this->ui->ens_txchannel->setText(QVariant::fromValue<quint8>(ns.lastTXChannel).toString());
    this->ui->ens_ackchannel->setText(QVariant::fromValue<quint8>(ns.ackChannel).toString());
    this->ui->ens_routeSpeed->setText(ns.routeSpeed);
    this->ui->ens_routeScheme->setText(ns.routeScheme);
}

void MainWindow::openLogWindow() {
    this->m_logWindow.show();
}

void MainWindow::openMetaDataWindow() {
    qCDebug(ozwadmin) << "Opening Window";
    QModelIndex index = this->ui->nodeList->currentIndex();
    const QAbstractItemModel *model = index.model();
    quint8 node = model->data(model->index(index.row(), QTOZW_Nodes::NodeColumns::NodeID)).value<quint8>();
    MetaDataWindow *mdwin = new MetaDataWindow(this);
    mdwin->populate(this->m_qtozwmanager, node);
    mdwin->setModal(true);
    mdwin->exec();
}

void MainWindow::OpenDeviceDB() {
    DeviceDB *ddb = new DeviceDB();
    ddb->show();
}

void MainWindow::openConfigWindow() {
    Configuration *cfg = new Configuration(this->m_qtozwmanager->getOptions(), this);
    cfg->show();
}
