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
	m_configpath = QFileInfo(QSettings().value("openzwave/ConfigPath").toString()).absoluteFilePath();
	m_userpath = QFileInfo(QSettings().value("openzwave/UserPath").toString()).absoluteFilePath();
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

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
    bool ok;
    int size = qEnvironmentVariableIntValue("QTRO_NODES_CACHE_SIZE", &ok);
    if (!ok) {
        qCDebug(ozwadmin) << "Using Saved Network Cache Size:" << networkCache();
        setNetworkCache(networkCache());
    } else {
        qCDebug(ozwadmin) << "Using Enviroment Network Cache:" << size;
        setNetworkCache(size);
    }
    /* Just Put our Enviroment Back there in case it was default */
    qputenv("QTRO_NODES_CACHE_SIZE", QString::number(networkCache()).toLocal8Bit());

    setLogBufferSize(logBufferSize());
    setRetriveLogBuffer(retriveLogBuffer());

}
SettingsManager::~SettingsManager() 
{

}

quint32 SettingsManager::networkCache() 
{
    return m_settings.value("network/cachesize", 1000).toInt();
}

void SettingsManager::setNetworkCache(QVariant size) 
{
    qCInfo(ozwadmin) << "setNetworkCache Variant Called";
    setNetworkCache(size.toInt());
}

void SettingsManager::setNetworkCache(quint32 size) 
{
    if (size != networkCache()) {
        m_settings.setValue("network/cachesize", size);
        emit networkCacheChanged(size);
        qCInfo(ozwadmin) << "Set Network Cache Size Called: " << QString::number(size).toLocal8Bit();
        qputenv("QTRO_NODES_CACHE_SIZE", QString::number(size).toLocal8Bit());
        bool ok;
        int size = qEnvironmentVariableIntValue("QTRO_NODES_CACHE_SIZE", &ok);
        if (!ok) {
            qCDebug(ozwadmin) << "QTRO_NODES_CACHE_SIZE Enviroment missing";
        } else {
            qCDebug(ozwadmin) << "QTRO_NODES_CACHE_SIZE Enviroment is now set to" << size;
        }
    }
}

quint32 SettingsManager::logBufferSize() 
{
    return m_settings.value("network/logbuffersize", 10000).toInt();
}

void SettingsManager::setLogBufferSize(QVariant size) 
{
    qCInfo(ozwadmin) << "setLogBufferSize Variant Called";
    setLogBufferSize(size.toInt());
}

void SettingsManager::setLogBufferSize(quint32 size) 
{
    if (size != logBufferSize()) {
        m_settings.setValue("network/logbuffersize", size);
        emit logBufferSizeChanged(size);
    }
}

bool SettingsManager::retriveLogBuffer() 
{
    return m_settings.value("network/retrivelogbuffer", true).toBool();
}

void SettingsManager::setRetriveLogBuffer(QVariant value) 
{
    qCInfo(ozwadmin) << "setRetriveLogBuffer Variant Called" << value;
    setRetriveLogBuffer(value.toBool());
}

void SettingsManager::setRetriveLogBuffer(bool value) 
{
    if (value != retriveLogBuffer()) {
        m_settings.setValue("network/retrivelogbuffer", value);
        emit retriveLogBufferChanged(value);
    }
}

