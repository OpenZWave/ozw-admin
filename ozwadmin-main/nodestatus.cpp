#include <qt-openzwave/qtopenzwave.h>
#include <qt-openzwave/qtozwmanager.h>
#include "nodestatus.h"
#include "ui_nodestatus.h"
#include "ozwcore.h"

NodeStatus::NodeStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodeStatus)
{
    ui->setupUi(this);
	QObject::connect(&this->m_statTimer, &QTimer::timeout, this, &NodeStatus::updateNodeStats);

}

NodeStatus::~NodeStatus()
{
    delete ui;
}

void NodeStatus::updateNodeStats() {
	if (OZWCore::get()->getQTOZWManager()->isReady()) {
		if (!this->m_statTimer.isActive())
			this->m_statTimer.start(1000);
	} else {
		this->m_statTimer.stop();
	}


	const QAbstractItemModel * model = this->currentNode.model();
	quint8 node = model->data(model->index(this->currentNode.row(), QTOZW_Nodes::NodeColumns::NodeID)).value<quint8>();
	QBitArray flags = model->data(model->index(this->currentNode.row(), QTOZW_Nodes::NodeColumns::NodeFlags)).value<QBitArray>();
	this->ui->ns_querystage->setText(OZWCore::get()->getQTOZWManager()->GetNodeQueryStage(node));
	this->ui->ns_sleeping->setChecked(!flags.at(QTOZW_Nodes::nodeFlags::isAwake));

	if (flags.at(QTOZW_Nodes::nodeFlags::isFailed)) {
		this->ui->ns_status->setText("Dead");
	}
	else if (!flags.at(QTOZW_Nodes::nodeFlags::isAwake)) {
		this->ui->ns_status->setText("Sleeping");
	}
	else {
		this->ui->ns_status->setText("Awake");
	}

	NodeStatistics ns = OZWCore::get()->getQTOZWManager()->GetNodeStatistics(node);
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

void NodeStatus::NodeSelected(QModelIndex current, QModelIndex previous) {
	Q_UNUSED(previous);
	if (!current.isValid()) {
		return;
	}
	this->currentNode = current;
	updateNodeStats();
}