#include <QMessageBox>
#include <QIntValidator>

#include "startup.h"
#include "ui_startup.h"


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
	this->m_startServer = false;
	this->setResult(DialogCode::Accepted);
	this->accept();
}
void Startup::remotePressed() {
    if (ui->remotehost->text().length() == 0) {
        QMessageBox::critical(this, tr("Error"), tr("You Must Specify a Remote Host"), QMessageBox::Ok);
    }
	this->m_remoteHost = ui->remotehost->text();
	this->m_remotePort = ui->remoteport->text().toUInt();
	this->m_remote = true;
	this->setResult(DialogCode::Accepted);
	this->accept();
}

