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
#include <QDataWidgetMapper>

#include <qt-openzwave/qtozwproxymodels.h>
#include <qt-openzwave/qtozw_pods.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "metadatawindow.h"
#include "logwindow.h"
#include "devicedb.hpp"

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

#if 0
    this->logBrowser = new LogBrowser;
    connect(ui->actionOpen_Log_Window, SIGNAL(triggered()), this, SLOT(openLogWindow()));
#endif

    connect(ui->actionOpen_Serial_Port, SIGNAL(triggered()), this, SLOT(OpenSerialPort()));
    connect(ui->actionDevice_Database, SIGNAL(triggered()), this, SLOT(OpenDeviceDB()));
    connect(ui->md_helpwindow, &QPushButton::clicked, this, &MainWindow::openMetaDataWindow);
    this->ui->nodeList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->nodeList->resizeColumnsToContents();
    this->ui->nodeList->horizontalHeader()->setStretchLastSection(true);
    this->ui->nodeList->verticalHeader()->hide();
    this->ui->nodeList->setSelectionMode(QAbstractItemView::SingleSelection);

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

    this->m_serialport = settings.value("SerialPort", "/dev/ttyUSB0").toString();


    SetReadOnly(this->ui->ni_flirs, true);
    SetReadOnly(this->ui->ni_listen, true);
    SetReadOnly(this->ui->ni_zwplus, true);
    SetReadOnly(this->ui->ni_beaming, true);
    SetReadOnly(this->ui->ni_routing, true);
    SetReadOnly(this->ui->ni_security, true);


//    printf("Starting OZWAdmin with OpenZWave Version %s\n", OpenZWave::Manager::getVersionAsString().c_str());

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
    this->m_openzwave = new QTOpenZwave(this);
    this->m_qtozwmanager = this->m_openzwave->GetManager();
    QObject::connect(this->m_qtozwmanager, &QTOZWManager::ready, this, &MainWindow::QTOZW_Ready);
    this->m_qtozwmanager->initilizeSource(this->settings.value("StartServer").toBool());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::QTOZW_Ready() {
    qDebug() << "QTOZW Ready";
    QTOZW_proxyNodeModel *nodeList = new QTOZW_proxyNodeModel(this);
    nodeList->setSourceModel(this->m_qtozwmanager->getNodeModel());
    this->ui->nodeList->setModel(nodeList);
    for (int i = 0; i <= QTOZW_Nodes::NodeColumns::NodeCount; i++) {
        switch (i) {
            case QTOZW_Nodes::NodeColumns::NodeID:
            case QTOZW_Nodes::NodeColumns::NodeName:
            case QTOZW_Nodes::NodeColumns::NodeProductName:
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

    QTOZW_proxyValueModel *configList = new QTOZW_proxyValueModel(this);
    configList->setSourceModel(this->m_qtozwmanager->getValueModel());
    configList->setFilterGenre(QTOZW_ValueIds::ValueIdGenres::Config);
    configList->setSelectionModel(this->ui->nodeList->selectionModel());
    this->ui->val_config_tbl->setModel(configList);

    QTOZW_proxyValueModel *systemList = new QTOZW_proxyValueModel(this);
    systemList->setSourceModel(this->m_qtozwmanager->getValueModel());
    systemList->setFilterGenre(QTOZW_ValueIds::ValueIdGenres::System);
    systemList->setSelectionModel(this->ui->nodeList->selectionModel());
    this->ui->val_system_tbl->setModel(systemList);

}


void MainWindow::OpenSerialPort() {

    bool ok;
    QString text = QInputDialog::getText(this, tr("Select Serial Port"),
                                         tr("Serial Port:"), QLineEdit::Normal,
                                         this->m_serialport, &ok);
    if (!ok)
        return;

    /* check the port exists */
    QFileInfo check_port(text.trimmed());
    if (!check_port.exists() && !check_port.isWritable()) {
        QMessageBox::warning(this, tr("OZW-Admin"),
                             tr("The Serial Port does not exist or is not accessable"),
                             QMessageBox::Ok);
        return;
    }

    this->m_serialport = text.trimmed();
    this->settings.setValue("SerialPort", this->m_serialport);
    this->m_qtozwmanager->open(this->m_serialport);

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
    this->ui->ns_status->setText(this->m_qtozwmanager->GetNodeQueryStage(node));
    NodeStatistics ns = this->m_qtozwmanager->GetNodeStatistics(node);
    this->ui->ns_lastseen->setText(ns.lastReceivedTimeStamp);
}


void MainWindow::openLogWindow() {

//    this->logBrowser->show();
}

void MainWindow::openMetaDataWindow() {
    qDebug() << "Opening Window";
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
