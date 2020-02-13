
#include "startupprogress.h"
#include "ui_startupprogress.h"
#include "util.h"


startupprogress::startupprogress(bool remote, QWidget *parent) :
	m_remote(remote),
	QDialog(parent),
    ui(new Ui::startupprogress)
{
    ui->setupUi(this);
	ui->progressBar->setValue(0);
	ui->label->setText("Starting....");
}

startupprogress::~startupprogress()
{
    delete ui;
}

void startupprogress::setQTOZWManager(QTOZWManager *qtozw) 
{
	m_qtozwmanager = qtozw;

	/* connect the signals */
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::manufacturerSpecificDBReady, this, &startupprogress::manufacturerSpecificDBReady);
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::ready, this, &startupprogress::ready);
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::starting, this, &startupprogress::starting);
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::started, this, &startupprogress::started);
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::driverReady, this, &startupprogress::driverReady);
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::driverAllNodesQueriedSomeDead, this, &startupprogress::driverAllNodesQueriedSomeDead);
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::driverAllNodesQueried, this, &startupprogress::driverAllNodesQueried);
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::driverAwakeNodesQueried, this, &startupprogress::driverAwakeNodesQueried);
	QObject::connect(this->m_qtozwmanager, &QTOZWManager::ozwNotification, this, &startupprogress::ozwNotification);

}

void startupprogress::manufacturerSpecificDBReady() {
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("Manufacturer Specific Database Ready");
}

void startupprogress::ready() {
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("OpenZWave Ready");
}

void startupprogress::starting() {
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("OpenZWave Starting");
}
void startupprogress::started(quint32 homeID) {
	Q_UNUSED(homeID);
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("OpenZWave Started");

}
void startupprogress::driverReady(quint32 homeID) {
	Q_UNUSED(homeID);
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("Driver Ready!");

}
void startupprogress::driverFailed(quint32 homeID) {
	Q_UNUSED(homeID);

}
void startupprogress::driverReset(quint32 homeID) {
	Q_UNUSED(homeID);

}
void startupprogress::driverRemoved(quint32 homeID) {
	Q_UNUSED(homeID);

}
void startupprogress::driverAllNodesQueriedSomeDead() {
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("All Nodes Queried - Some Dead");
	this->close();
}
void startupprogress::driverAllNodesQueried() {
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("All Nodes Queried");
	this->close();
}
void startupprogress::driverAwakeNodesQueried() {
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("Awake Nodes Queried");
	this->close();
}
void startupprogress::ozwNotification(quint8 node, NotificationTypes::QTOZW_Notification_Code event) {
	qCDebug(ozwadmin) << event;
}