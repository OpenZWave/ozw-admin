#include <QDebug>
#include <QMessageBox>

#include <qt-openzwave/qt-openzwavedatabase.h>

#include "ozwcore.h"
#include "util.h"

Q_GLOBAL_STATIC(OZWCore, globalState)

OZWCore::OZWCore(QObject *parent) : 
    QObject(parent)
{
}

OZWCore *OZWCore::get() {
    return globalState();
}


void OZWCore::initilize() {
	m_configpath = QFileInfo(settings.value("openzwave/ConfigPath").toString()).absoluteFilePath();
	m_userpath = QFileInfo(settings.value("openzwave/UserPath").toString()).absoluteFilePath();
    this->m_openzwave = new QTOpenZwave(this, m_configpath, m_userpath);
    this->m_qtozwmanager = this->m_openzwave->GetManager();
}

QTOpenZwave *OZWCore::getQTOZW() {
    return this->m_openzwave;
}
QTOZWManager *OZWCore::getQTOZWManager() {
    return this->m_qtozwmanager;
}
