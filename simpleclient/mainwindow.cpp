#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startup.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->m_openzwave = new QTOpenZwave(this);
    this->m_qtozwmanager = this->m_openzwave->GetManager();
    QObject::connect(this->m_qtozwmanager, &QTOZWManager::ready, this, &MainWindow::QTOZW_Ready);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start() {
    Startup win(this);
    win.setModal(true);
    QObject::connect(&win, &Startup::startLocal, this, &MainWindow::startLocal);
    QObject::connect(&win, &Startup::startRemote, this, &MainWindow::startRemote);
    win.exec();
}

void MainWindow::startRemote(QString url) {
    QUrl server;
    server.setHost(url);
    server.setPort(1983);
    server.setScheme("tcp");
    qDebug() << server;
    this->m_qtozwmanager->initilizeReplica(server);

}
void MainWindow::startLocal(QString serialPort, bool startServer) {
    this->m_serialPort = serialPort;
    this->m_qtozwmanager->initilizeSource(startServer);
    qDebug() << "startLocal: " << this->m_serialPort;
}

void MainWindow::QTOZW_Ready() {
    qDebug() << "OZW Ready " << this->m_serialPort;
    if (this->m_qtozwmanager->isRunning() == false) {
        this->m_qtozwmanager->open(this->m_serialPort);
    }
    this->ui->nodeView->setModel(this->m_qtozwmanager->getNodeModel());
    this->ui->userView->setModel(this->m_qtozwmanager->getValueModel());
    this->ui->configView->setModel(this->m_qtozwmanager->getValueModel());
    this->ui->systemView->setModel(this->m_qtozwmanager->getValueModel());
    this->ui->AssociationView->setModel(this->m_qtozwmanager->getAssociationModel());
}

