#include <QAbstractItemModel>
#include <qt-openzwave/qtozwmanager.h>
#include <qt-openzwave/qtozwnodemodel.h>
#include "deviceinfo.h"
#include "ui_deviceinfo.h"


void SetReadOnly(QCheckBox* checkBox, bool readOnly)
{
	checkBox->setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
	checkBox->setFocusPolicy(readOnly ? Qt::NoFocus : Qt::StrongFocus);
}

DeviceInfo::DeviceInfo(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DeviceInfo)
{
    ui->setupUi(this);
	SetReadOnly(this->ui->ni_flirs, true);
	SetReadOnly(this->ui->ni_listen, true);
	SetReadOnly(this->ui->ni_zwplus, true);
	SetReadOnly(this->ui->ni_beaming, true);
	SetReadOnly(this->ui->ni_routing, true);
	SetReadOnly(this->ui->ni_security, true);
	connect(this->ui->md_helpwindow, &QPushButton::clicked, this, &DeviceInfo::openMetaDataWindow);
}

DeviceInfo::~DeviceInfo()
{
    delete ui;
}

void DeviceInfo::setQTOZWManager(QTOZWManager *manager) {
	this->m_qtozwmanager = manager;

}


void DeviceInfo::NodeSelected(QModelIndex current, QModelIndex previous) {
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
		this->ui->md_ozwinfo->setText("<a href='" + url.toEncoded() + "'>" + tr("OpenZWave Database") + "</a>");
	else
		this->ui->md_ozwinfo->setText("");

	url = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ZWAProductURL);
	if (url.isValid())
		this->ui->md_zwainfo->setText("<a href='" + url.toEncoded() + "'>" + tr("ZWave Alliance Database") + "</a>");
	else
		this->ui->md_zwainfo->setText("");

	url = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ProductManualURL);
	if (url.isValid())
		this->ui->md_manual->setText("<a href='" + url.toEncoded() + "'>" + tr("Product Manual") + "</a>");
	else
		this->ui->md_manual->setText("");

	url = this->m_qtozwmanager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ProductSupportURL);
	if (url.isValid())
		this->ui->md_prodpage->setText("<a href='" + url.toEncoded() + "'>" + tr("Product Support") + "</a>");
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

}