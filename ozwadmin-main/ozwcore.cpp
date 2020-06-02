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
#ifndef Q_OS_WASM
	QStringList PossibleDBPaths;
	PossibleDBPaths << settings.value("openzwave/ConfigPath", QDir::toNativeSeparators("../../../config/")).toString().append("/");
	PossibleDBPaths << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);

	QString path, dbPath, userPath;
	foreach(path, PossibleDBPaths) {
		qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path + "/config/manufacturer_specific.xml")).absoluteFilePath() << " for manufacturer_specific.xml";
		if (QFileInfo(QDir::toNativeSeparators(path + "/config/manufacturer_specific.xml")).exists()) {
			dbPath = QFileInfo(QDir::toNativeSeparators(path + "/config/manufacturer_specific.xml")).absoluteFilePath();
			break;
		}
		qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path + "../config/manufacturer_specific.xml")).absoluteFilePath() << " for manufacturer_specific.xml";
		if (QFile(QDir::toNativeSeparators(path + "/../config/manufacturer_specific.xml")).exists()) {
			dbPath = QFileInfo(QDir::toNativeSeparators(path + "/../config/manufacturer_specific.xml")).absoluteFilePath();
			break;
		}
	}
	PossibleDBPaths.clear();
	PossibleDBPaths << settings.value("openzwave/UserPath", QDir::toNativeSeparators("../../../config/")).toString().append("/");
	PossibleDBPaths << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);

	foreach(path, PossibleDBPaths) {
		qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path + "/config/Options.xml")).absoluteFilePath() << " for Options.xml";
		if (QFileInfo(QDir::toNativeSeparators(path + "/config/Options.xml")).exists()) {
			userPath = QFileInfo(QDir::toNativeSeparators(path + "/config/Options.xml")).absoluteFilePath();
			break;
		}
		qCDebug(ozwadmin) << "Checking " << QFileInfo(QDir::toNativeSeparators(path + "/../config/Options.xml")).absoluteFilePath() << " for Options.xml";
		if (QFile(QDir::toNativeSeparators(path + "/../config/Options.xml")).exists()) {
			userPath = QFileInfo(QDir::toNativeSeparators(path + "/../config/Options.xml")).absoluteFilePath();
			break;
		}
	}

	qCDebug(ozwadmin) << "DBPath: " << dbPath;
	qCDebug(ozwadmin) << "userPath: " << userPath;

	if (dbPath.isEmpty()) {
		qCInfo(ozwadmin) << "Deploying OZW Database to " << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).at(0);
		QStringList paths;
		paths << "." << "../../qt-openzwave/qt-openzwavedatabase/";
		if (!initConfigDatabase(paths)) {
            qCWarning(ozwadmin) << "The qt-openzwavedatabase.rcc file could not be found";
            emit raiseCriticalError("Missing qt-openzwavedatabase.rcc Database File", "The qt-openzwavedatabase.rcc file could not be found");
			exit(-1);
		}
		QString dir = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).at(0);
		if (copyConfigDatabase(QDir(dir).absolutePath())) {
			qCInfo(ozwadmin) << "Copied Database to " << dir;
		}
		else {
            qCWarning(ozwadmin) << "Copying Config Database Failed";
			emit raiseCriticalError("Copying Config Database Failed", "Copying Config Database Failed");
			exit(-1);
		}
		dbPath = QFileInfo(dir.append("/config/")).absolutePath();
		m_configpath.setPath(dbPath);
		settings.setValue("openzwave/ConfigPath", m_configpath.absolutePath());
		qCInfo(ozwadmin) << "m_configPath set to " << m_configpath.absolutePath();
	}
	else
	{
		m_configpath.setPath(QFileInfo(dbPath).absolutePath());
		settings.setValue("openzwave/ConfigPath", m_configpath.absolutePath());
		qCInfo(ozwadmin) << "Found Existing DB Path" << m_configpath.absolutePath();
	}

	if (userPath.isEmpty()) {
		userPath = dbPath;
		m_userpath.setPath(QFileInfo(userPath).absolutePath());
		settings.setValue("openzwave/UserPath", m_userpath.absolutePath());
		qCInfo(ozwadmin) << "UserPath is Set to DBPath: " << m_userpath.absolutePath();
	}
	else {
		m_userpath.setPath(QFileInfo(userPath).absolutePath());
		qCInfo(ozwadmin) << "UserPath is Set from Settings" << m_userpath.absolutePath();
		settings.setValue("openzwave/UserPath", m_userpath.absolutePath());
	}
#else 
	m_configpath = ".";
	m_userpath = ".";
#endif
    this->m_openzwave = new QTOpenZwave(this, m_configpath, m_userpath);
    this->m_qtozwmanager = this->m_openzwave->GetManager();
}

QTOpenZwave *OZWCore::getQTOZW() {
    return this->m_openzwave;
}
QTOZWManager *OZWCore::getQTOZWManager() {
    return this->m_qtozwmanager;
}
