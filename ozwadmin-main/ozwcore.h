#ifndef OZWCORE_H
#define OZWCORE_H

#include <QObject>
#include <QMessageBox>
#include <qt-openzwave/qtopenzwave.h>
#include <qt-openzwave/qtozwmanager.h>

#include "controllercommands.h"

class ControllerCommands;

class SettingsManager : public QObject
{
    Q_OBJECT
    public:
        SettingsManager(QObject *parent = nullptr);
        ~SettingsManager();
        Q_PROPERTY(quint32 networkCache READ networkCache WRITE setNetworkCache NOTIFY networkCacheChanged);
        Q_PROPERTY(quint32 logBufferSize READ logBufferSize WRITE setLogBufferSize NOTIFY logBufferSizeChanged);
        Q_PROPERTY(bool retriveLogBuffer READ retriveLogBuffer WRITE setRetriveLogBuffer NOTIFY retriveLogBufferChanged);
        quint32 networkCache();
        quint32 logBufferSize();
        bool retriveLogBuffer();

    public Q_SLOTS:
        void setNetworkCache(QVariant size);
        void setNetworkCache(quint32 size);
        void setLogBufferSize(QVariant size);
        void setLogBufferSize(quint32 size);
        void setRetriveLogBuffer(QVariant value);
        void setRetriveLogBuffer(bool value);

    Q_SIGNALS:
        void networkCacheChanged(int size);
        void logBufferSizeChanged(quint32 size);
        void retriveLogBufferChanged(bool value);
        
    private:

        QSettings m_settings;        
};

class OZWCore : public QObject
{
    Q_OBJECT
public:
    explicit OZWCore(QObject *parent = nullptr);
    static OZWCore *get();
    void initilize();
    QTOpenZwave *getQTOZW();
    QTOZWManager *getQTOZWManager();
    SettingsManager settings;
signals:
    QMessageBox::StandardButton raiseCriticalError(QString title, QString message);
public slots:

private:
    QTOpenZwave *m_openzwave;
    QTOZWManager *m_qtozwmanager;
    QString m_serialport;
    QString m_configpath;
    QString m_userpath;
};



#endif // OZWCORE_H
