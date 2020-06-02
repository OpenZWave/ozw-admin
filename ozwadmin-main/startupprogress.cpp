#include <QObject>

#include "startupprogress.h"
#include "ui_startupprogress.h"
#include "util.h"
#include "ozwcore.h"


startupprogress::startupprogress(bool remote, QWidget *parent) :
	QDialog(parent),
    ui(new Ui::startupprogress),
    m_remote(remote)
{
    ui->setupUi(this);
	ui->progressBar->setValue(0);
	ui->label->setText("Starting....");

	/* connect the signals */
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::manufacturerSpecificDBReady, this, &startupprogress::manufacturerSpecificDBReady);
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::ready, this, &startupprogress::ready);
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::starting, this, &startupprogress::starting);
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::started, this, &startupprogress::started);
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::driverReady, this, &startupprogress::driverReady);
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::driverAllNodesQueriedSomeDead, this, &startupprogress::driverAllNodesQueriedSomeDead);
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::driverAllNodesQueried, this, &startupprogress::driverAllNodesQueried);
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::driverAwakeNodesQueried, this, &startupprogress::driverAwakeNodesQueried);
	QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::ozwNotification, this, &startupprogress::ozwNotification);
    QObject::connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::remoteConnectionStatus, this, &startupprogress::remoteConnectionStatus);
    QObject::connect(this->ui->cancelbtn, &QPushButton::clicked, this, &startupprogress::clicked);

}

startupprogress::~startupprogress()
{
    delete ui;
}

void startupprogress::clicked(bool checked) {
    Q_UNUSED(checked);
    emit this->cancel();
    this->close();
}
void startupprogress::manufacturerSpecificDBReady() {
	ui->progressBar->setValue(ui->progressBar->value() + 5);
	ui->label->setText("Manufacturer Specific Database Ready");
}

void startupprogress::ready() {
	ui->label->setText("OpenZWave Ready");
    this->close();
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
	Q_UNUSED(node);
	qCDebug(ozwadmin) << event;
}

void startupprogress::remoteConnectionStatus(QTOZWManager::connectionStatus status, QAbstractSocket::SocketError error) {
    qCDebug(ozwadmin) << status << error;
    if (status != QTOZWManager::connectionStatus::ConnectionErrorState) {
        QMetaEnum metaEnum = QMetaEnum::fromType<QTOZWManager::connectionStatus>();
        ui->label->setText(metaEnum.valueToKey(status));
        if ((status >= QTOZWManager::connectionStatus::GotManagerData) && (status <= QTOZWManager::connectionStatus::GotLogData)) {
            ui->progressBar->setValue(ui->progressBar->value()+16);
            qCDebug(ozwadmin) << ui->progressBar->value();
        }
    } else {
        QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
        ui->label->setText(QString("Error: ").append(metaEnum.valueToKey(error)));
    }
}
