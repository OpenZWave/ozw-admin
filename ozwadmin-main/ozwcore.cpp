#include <QDebug>
#include <QMessageBox>

#include <qt-openzwave/qt-openzwavedatabase.h>

#include "ozwcore.h"
#include "util.h"

Q_GLOBAL_STATIC(OZWCore, globalState)


OZWCore::OZWCore(QObject *parent) : 
    QObject(parent),
    m_openzwave(nullptr),
    m_qtozwmanager(nullptr)
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
    qCInfo(ozwadmin) << "OZWCore Initilized" << this->m_openzwave << this->m_qtozwmanager;
}

QTOpenZwave *OZWCore::getQTOZW() {
    if (!this->m_openzwave)
        qCWarning(ozwadmin) << "QTOpenZWave is not Created!";
    return this->m_openzwave;
}
QTOZWManager *OZWCore::getQTOZWManager() {
    if (!this->m_qtozwmanager)
        qCWarning(ozwadmin) << "QTOZWManager is not created!";
    return this->m_qtozwmanager;
}
