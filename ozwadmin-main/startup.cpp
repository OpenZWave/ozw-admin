#include <QMessageBox>
#include <QIntValidator>
#include <QSettings>
#include <QUrl>
#include <QDebug>
#include <QSerialPortInfo>

#include "startup.h"
#include "ui_startup.h"
#include "ozwcore.h"
#include "util.h"

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
    QString selected;
    foreach(QSerialPortInfo spinfo, QSerialPortInfo::availablePorts()) {
#if defined(Q_OS_MACOS)
        if (spinfo.portName().startsWith("tty", Qt::CaseInsensitive))
            continue;
#endif
        ui->serialport->addItem(spinfo.portName(), spinfo.systemLocation());
        if (QSettings().value("connection/serialport") == spinfo.systemLocation())
            selected = spinfo.portName();
    }
    ui->serialport->setCurrentText(selected);
    //ui->enableserver->setChecked(QSettings().value("connection/startserver", true).toBool());
    ui->enableserver->setChecked(true);
    QUrl server = QUrl::fromUserInput(QSettings().value("connection/remotehost", "ws://localhost:1983").toString());
    ui->remotehost->setText(server.host());
    ui->remoteport->setText(QString::number(server.port()));
    ui->authKey->setText(QSettings().value("connection/authKey", "").toString());
}

Startup::~Startup()
{
    delete ui;
}

void Startup::localPressed() {

	this->m_serialPort = ui->serialport->currentData().toString();
	this->m_remote = false;
    this->m_startServer = ui->enableserver->isChecked();
	this->setResult(DialogCode::Accepted);
	this->accept();
}
void Startup::remotePressed() {
    if (ui->remotehost->text().length() == 0) {
        QMessageBox::critical(this, tr("Error"), tr("You Must Specify a Remote Host"), QMessageBox::Ok);
        this->reject();
        return;
    }
	this->m_remoteHost = ui->remotehost->text();
	this->m_remotePort = ui->remoteport->text().toUInt();
    this->m_authKey = ui->authKey->text();
	this->m_remote = true;
	this->setResult(DialogCode::Accepted);
	this->accept();
}

