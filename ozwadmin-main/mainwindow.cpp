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
#include <QFileDialog>
#include <QStandardPaths>



#include <qt-openzwave/qtozwoptions.h>
#include <qt-openzwave/qtozw_pods.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "splashdialog.h"
#include "metadatawindow.h"
#include "logwindow.h"
#include "devicedb.hpp"
#include "configuration.h"
#include "startup.h"
#include "startupprogress.h"
#include "util.h"
#include "deviceinfo.h"
#include "nodestatus.h"
#include "valuetable.h"
#include "nodeflagswidget.h"
#include "qt-ads/DockAreaWidget.h"
#include "ozwcore.h"
#include "eventwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	connect(OZWCore::get(), &OZWCore::raiseCriticalError, this, &MainWindow::openCriticalDialog, Qt::DirectConnection);
	OZWCore::get()->initilize();

	this->sbMsg = new statusBarMessages(this);
	connect(this->sbMsg, &statusBarMessages::newMessage, this, &MainWindow::setStatusBarMsg);


	this->ui->setupUi(this);
	this->m_DockManager = new ads::CDockManager(this);
	this->m_controllerCommands = new ControllerCommands(this);
	this->connected(false);

	DeviceInfo *di = new DeviceInfo(this);
	NodeStatus *ni = new NodeStatus(this);
	EventWindow *ew = new EventWindow(this);
	LogWindow *lw = new LogWindow(this);
	connect(this->sbMsg, &statusBarMessages::newMessage, ew, &EventWindow::newEvent);


	statusBar()->showMessage(tr("Starting..."));
	this->ui->action_Close->setEnabled(false);

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OpenConnection()));
	connect(ui->action_Close, SIGNAL(triggered()), this, SLOT(CloseConnection()));
	connect(ui->actionDevice_Database, SIGNAL(triggered()), this, SLOT(OpenDeviceDB()));
	connect(ui->action_Configuration, SIGNAL(triggered()), this, SLOT(openConfigWindow()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openAboutWindow()));
	connect(ui->action_AddNode, SIGNAL(triggered()), this, SLOT(addNode()));
	connect(ui->action_Delete_Node, SIGNAL(triggered()), this, SLOT(delNode()));
	connect(ui->action_Heal_Network, SIGNAL(triggered()), this, SLOT(healNetwork()));


	connect(di, &DeviceInfo::openMetaDataWindow, this, &MainWindow::openMetaDataWindow);

	this->ntw = new nodeTableWidget(this);
	connect(this->ntw, &nodeTableWidget::currentRowChanged, this, &MainWindow::NodeSelected);
	connect(this->ntw, &nodeTableWidget::currentRowChanged, di, &DeviceInfo::NodeSelected);
	connect(this->ntw, &nodeTableWidget::currentRowChanged, ni, &NodeStatus::NodeSelected);
	
	
	ads::CDockWidget* NodeListDW = new ads::CDockWidget("Node List");
	NodeListDW->setWidget(this->ntw);
	this->m_DockManager->addDockWidget(ads::LeftDockWidgetArea, NodeListDW);

	ads::CDockWidget* DeviceInfoDW = new ads::CDockWidget("Node Info");
	DeviceInfoDW->setWidget(di);
	auto RightDockWidget = this->m_DockManager->addDockWidget(ads::RightDockWidgetArea, DeviceInfoDW);
	
	ads::CDockWidget* DeviceStatusDW = new ads::CDockWidget("Node Status");
	DeviceStatusDW->setWidget(ni);
	this->m_DockManager->addDockWidget(ads::CenterDockWidgetArea, DeviceStatusDW, RightDockWidget);

	ValueTable *userValues = new ValueTable(QTOZW_ValueIds::ValueIdGenres::User, this);
	ads::CDockWidget* userValueDW = new ads::CDockWidget("User Values");
	userValueDW->setWidget(userValues);
	this->m_DockManager->addDockWidget(ads::CenterDockWidgetArea, userValueDW, RightDockWidget);

	ValueTable *systemValues = new ValueTable(QTOZW_ValueIds::ValueIdGenres::System, this);
	ads::CDockWidget *systemValueDW = new ads::CDockWidget("System Values");
	systemValueDW->setWidget(systemValues);
	this->m_DockManager->addDockWidget(ads::CenterDockWidgetArea, systemValueDW, RightDockWidget);

	ValueTable *configValues = new ValueTable(QTOZW_ValueIds::ValueIdGenres::Config, this);
	ads::CDockWidget *configValueDW = new ads::CDockWidget("Config Values");
	configValueDW->setWidget(configValues);
	this->m_DockManager->addDockWidget(ads::CenterDockWidgetArea, configValueDW, RightDockWidget);

	RightDockWidget->setCurrentDockWidget(DeviceInfoDW);

	ads::CDockWidget *eventViewDW = new ads::CDockWidget("Event List");
	eventViewDW->setWidget(ew);
	auto BottomDockWidget = this->m_DockManager->addDockWidget(ads::BottomDockWidgetArea, eventViewDW);

	ads::CDockWidget *logWindowDW = new ads::CDockWidget("OZW Logs");
	logWindowDW->setWidget(lw);
	this->m_DockManager->addDockWidget(ads::CenterDockWidgetArea, logWindowDW, BottomDockWidget);
	BottomDockWidget->setCurrentDockWidget(eventViewDW);


	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::ready, this, &MainWindow::QTOZW_Ready);

    OZWCore::get()->getQTOZWManager()->initilizeSource(OZWCore::get()->settings.value("StartServer").toBool());

	userValues->setModel(OZWCore::get()->getQTOZWManager()->getValueModel(), this->ntw->selectionModel());
	systemValues->setModel(OZWCore::get()->getQTOZWManager()->getValueModel(), this->ntw->selectionModel());
	configValues->setModel(OZWCore::get()->getQTOZWManager()->getValueModel(), this->ntw->selectionModel());
	lw->setModel(OZWCore::get()->getQTOZWManager()->getLogModel());

	di->setQTOZWManager(OZWCore::get()->getQTOZWManager());
	ni->setQTOZWManager(OZWCore::get()->getQTOZWManager());

	SplashDialog *sw = new SplashDialog(OZWCore::get()->getQTOZW(), this);
	sw->show();
	sw->move(this->geometry().center() - sw->rect().center());

	QTimer::singleShot(5000, sw, SLOT(close()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::QTOZW_Ready() {
    qCDebug(ozwadmin) << "QTOZW Ready";

    /* apply our Local Configuration Options to the OZW Options Class */
    OZWCore::get()->settings.beginGroup("openzwave");
    QStringList optionlist = OZWCore::get()->settings.allKeys();
    for (int i = 0; i < optionlist.size(); i++) {
        qCDebug(ozwadmin) << "Updating Option " << optionlist.at(i) << " to " << OZWCore::get()->settings.value(optionlist.at(i));
        QTOZWOptions *ozwoptions = OZWCore::get()->getQTOZWManager()->getOptions();
        QStringList listtypes;
        listtypes << "SaveLogLevel" << "QueueLogLevel" << "DumpLogLevel";
        if (listtypes.contains(optionlist.at(i))) {
            OptionList list = ozwoptions->property(optionlist.at(i).toLocal8Bit()).value<OptionList>();
            if (list.getEnums().size() > 0)
                list.setSelected(OZWCore::get()->settings.value(optionlist.at(i)).toString());
        }
        else
        {
            ozwoptions->setProperty(optionlist.at(i).toLocal8Bit(), OZWCore::get()->settings.value(optionlist.at(i)));
        }
    }
    OZWCore::get()->settings.endGroup();

	this->ntw->setModel(OZWCore::get()->getQTOZWManager()->getNodeModel());
}

void MainWindow::OpenConnection() {
	this->connected(true);

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
            sup->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
			sup->setQTOZWManager(OZWCore::get()->getQTOZWManager());
			sup->show();
            OZWCore::get()->getQTOZWManager()->setClientAuth(su.getauthKey());
			OZWCore::get()->getQTOZWManager()->initilizeReplica(server);
            OZWCore::get()->settings.setValue("connection/remotehost", su.getremoteHost());
            OZWCore::get()->settings.setValue("connection/remoteport", su.getremotePort());
            OZWCore::get()->settings.setValue("connection/authKey", su.getauthKey());
			return;
		}
		else 
		{
			qCDebug(ozwadmin) << "Doing Local Connection: " << su.getserialPort() << su.getstartServer();
			startupprogress *sup = new startupprogress(false, this);
			sup->setQTOZWManager(OZWCore::get()->getQTOZWManager());
			sup->show();
			OZWCore::get()->getQTOZWManager()->open(su.getserialPort());
            OZWCore::get()->settings.setValue("connection/serialport", su.getserialPort());
            OZWCore::get()->settings.setValue("connection/startserver", su.getstartServer());
			return;
		}
    } else {
        qCDebug(ozwadmin) << "Open Dialog was Canceled" << ret;
		this->connected(false);
    }

}
void MainWindow::CloseConnection() {
	if (OZWCore::get()->getQTOZWManager()->getConnectionType() == QTOZWManager::connectionType::Local) {
		OZWCore::get()->getQTOZWManager()->close();
	} else if (OZWCore::get()->getQTOZWManager()->getConnectionType() == QTOZWManager::connectionType::Remote) {
		QMessageBox::critical(this, "Close Connection", "TODO: Please restart the application for now");
		exit(1);
	} else {
		QMessageBox::critical(this, "Unknown Connection Type", "Unknown Connection Type");
	}
	this->connected(false);
}



void MainWindow::resizeColumns() {
    //this->ui->nodeList->resizeColumnsToContents();
}



void MainWindow::NodeSelected(QModelIndex current,QModelIndex previous) {
    Q_UNUSED(previous);
	Q_UNUSED(current);
#if 0
	if (!current.isValid()) {
        return;
    }
    const QAbstractItemModel * model = current.model();

    quint8 node = model->data(model->index(current.row(), QTOZW_Nodes::NodeColumns::NodeID)).value<quint8>();
#endif
}


void MainWindow::openMetaDataWindow() {
    qCDebug(ozwadmin) << "Opening Window";
    QModelIndex index = this->ntw->currentIndex();
    const QAbstractItemModel *model = index.model();
    quint8 node = model->data(model->index(index.row(), QTOZW_Nodes::NodeColumns::NodeID)).value<quint8>();
    MetaDataWindow *mdwin = new MetaDataWindow(this);
    mdwin->populate(OZWCore::get()->getQTOZWManager(), node);
    mdwin->setModal(true);
    mdwin->exec();
}

void MainWindow::OpenDeviceDB() {
    DeviceDB *ddb = new DeviceDB();
    ddb->show();
}

void MainWindow::openConfigWindow() {
    Configuration *cfg = new Configuration(OZWCore::get()->getQTOZWManager()->getOptions(), this);
    cfg->show();
}

void MainWindow::openAboutWindow() {
	SplashDialog *sw = new SplashDialog(OZWCore::get()->getQTOZW(), this);
	sw->show();
	sw->move(this->geometry().center() - sw->rect().center());
}

QMessageBox::StandardButton MainWindow::openCriticalDialog(QString title, QString msg) {
	return QMessageBox::critical(this, title, msg);
}


void MainWindow::addNode() {
	this->m_controllerCommands->addNode();
}
void MainWindow::delNode() {
	this->m_controllerCommands->delNode();
}
void MainWindow::healNetwork() {
	this->m_controllerCommands->healNetwork();
}

void MainWindow::connected(bool connected) {
	this->ui->actionOpen->setEnabled(!connected);
    this->ui->action_Close->setEnabled(connected);
	this->ui->action_AddNode->setEnabled(connected);
	this->ui->action_Delete_Node->setEnabled(connected);
	this->ui->action_Heal_Network->setEnabled(connected);
}

void MainWindow::setStatusBarMsg(QString Msg) {
	 this->statusBar()->showMessage(QTime::currentTime().toString("hh:m:ss ap").append(" Event: ").append(Msg), 5000);
}
