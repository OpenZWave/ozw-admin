#include <QMessageBox>
#include <QIntValidator>
#include <QSettings>
#include <QUrl>
#include <QDebug>

#include "startup.h"
#include "ui_startup.h"
#include "ozwcore.h"

Startup::Startup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Startup)
{
    ui->setupUi(this);
    ui->remoteport->setValidator( new QIntValidator(1, 65535, this));
    QFontMetrics fm = ui->remoteport->fontMetrics();
    int w = fm.boundingRect(ui->remoteport->text()).width();
    ui->remoteport->setMaximumWidth(w+8);
    QObject::connect(ui->startlocal, &QPushButton::clicked, this, &Startup::localPressed);
    QObject::connect(ui->startremote, &QPushButton::clicked, this, &Startup::remotePressed);
#if defined(Q_OS_MACOS)
    ui->serialport->setText(QSettings().value("connection/serialport", "/dev/cu.SLAB_USBtoUART").toString());
#elif defined(Q_OS_WIN)
    ui->serialport->setText(QSettings().value("connection/serialport", "COM1").toString());
#else
    ui->serialport->setText(QSettings().value("connection/serialport", "/dev/ttyUSB0").toString());
#endif
    ui->enableserver->setChecked(QSettings().value("connection/startserver", true).toBool());
    QUrl server = QUrl::fromUserInput(QSettings().value("connection/remotehost", "ws://localhost:1983").toString());
    qDebug() << server;
    ui->remotehost->setText(server.host());
    ui->remoteport->setText(QString::number(server.port()));
    ui->authKey->setText(QSettings().value("connection/authKey", "").toString());
}

Startup::~Startup()
{
    delete ui;
}

void Startup::localPressed() {

    if (ui->serialport->text().length() == 0) {
        QMessageBox::critical(this, tr("Error"), tr("You Must Specify a Serial Port"), QMessageBox::Ok);
    }
	this->m_serialPort = ui->serialport->text();
	this->m_remote = false;
    this->m_startServer = ui->enableserver->isChecked();
	this->setResult(DialogCode::Accepted);
	this->accept();
}
void Startup::remotePressed() {
    if (ui->remotehost->text().length() == 0) {
        QMessageBox::critical(this, tr("Error"), tr("You Must Specify a Remote Host"), QMessageBox::Ok);
    }
	this->m_remoteHost = ui->remotehost->text();
	this->m_remotePort = ui->remoteport->text().toUInt();
    this->m_authKey = ui->authKey->text();
	this->m_remote = true;
	this->setResult(DialogCode::Accepted);
	this->accept();
}

