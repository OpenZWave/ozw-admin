#ifndef OZWCORE_H
#define OZWCORE_H

#include <QObject>
#include <QMessageBox>
#include <qt-openzwave/qtopenzwave.h>
#include <qt-openzwave/qtozwmanager.h>

#include "controllercommands.h"

class ControllerCommands;

class OZWCore : public QObject
{
    Q_OBJECT
public:
    explicit OZWCore(QObject *parent = nullptr);
    static OZWCore *get();
    void initilize();
    QTOpenZwave *getQTOZW();
    QTOZWManager *getQTOZWManager();
    QSettings settings;

signals:
    QMessageBox::StandardButton raiseCriticalError(QString title, QString message);
public slots:

private:
    QTOpenZwave *m_openzwave;
    QTOZWManager *m_qtozwmanager;
    QString m_serialport;
    QDir m_configpath;
    QDir m_userpath;
};



#endif // OZWCORE_H
