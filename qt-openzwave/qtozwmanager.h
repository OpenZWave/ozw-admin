#ifndef QTOZWMANAGER_H
#define QTOZWMANAGER_H

#include <QObject>
#include "rep_qtozwmanager_source.h"
#include "QTOZWNodes.h"

void setupOZW();

//Q_DECLARE_METATYPE(QList<OZWNodes>)

namespace OpenZWave {

/* forward Declaration */
class Manager;
class Options;
class Notification;
}

class QTOZWManager : public QTOZWManagerSimpleSource
{
public:
    friend void OZWNotification (OpenZWave::Notification const* _notification, void* _context);

    QTOZWManager();

    bool Start(QString SerialPort);

private:
    bool Lock();
    bool Unlock();

private:
    OpenZWave::Options *m_options;
    OpenZWave::Manager *m_manager;
    pthread_mutex_t m_manager_mutex;
};

#endif // QTOZWMANAGER_H
