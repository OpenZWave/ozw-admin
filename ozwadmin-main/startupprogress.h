#ifndef STARTUPPROGRESS_H
#define STARTUPPROGRESS_H

#include <QDialog>
#include <qt-openzwave/qtopenzwave.h>
#include <qt-openzwave/qtozwmanager.h>


namespace Ui {
class startupprogress;
}

class startupprogress : public QDialog
{
    Q_OBJECT

public:
    explicit startupprogress(bool remote, QWidget *parent = nullptr);
    ~startupprogress();
	void setQTOZWManager(QTOZWManager *);

Q_SIGNALS:
    void cancel();

public Q_SLOTS:
	void manufacturerSpecificDBReady();
	void ready();
	void starting();
	void started(quint32 homeID);
	void driverReady(quint32 homeID);
	void driverFailed(quint32 homeID);
	void driverReset(quint32 homeID);
	void driverRemoved(quint32 homeID);
	void driverAllNodesQueriedSomeDead();
	void driverAllNodesQueried();
	void driverAwakeNodesQueried();
	void ozwNotification(quint8 node, NotificationTypes::QTOZW_Notification_Code event);
    void remoteConnectionStatus(QTOZWManager::connectionStatus status, QAbstractSocket::SocketError error);
private Q_SLOTS:
    void clicked(bool checked);

private:
    Ui::startupprogress *ui;
	bool m_remote;
	QTOZWManager *m_qtozwmanager;
};

#endif // STARTUPPROGRESS_H
