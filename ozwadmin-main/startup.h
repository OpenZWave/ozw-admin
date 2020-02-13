#ifndef STARTUP_H
#define STARTUP_H

#include <QDialog>
#include <QObject>


namespace Ui {
class Startup;
}

class Startup : public QDialog
{
	Q_OBJECT

	Q_PROPERTY(QString serialPort MEMBER m_serialPort READ getserialPort);
	Q_PROPERTY(QString remotHost MEMBER m_remoteHost READ getremoteHost);
	Q_PROPERTY(quint32 reportPort MEMBER m_remotePort READ getremotePort);
	Q_PROPERTY(bool remote MEMBER m_remote READ getremote);
	Q_PROPERTY(bool startServer MEMBER m_startServer READ getstartServer);

public:
    explicit Startup(QWidget *parent = nullptr);
    ~Startup();
	QString getserialPort() { return m_serialPort; };
	QString getremoteHost() { return m_remoteHost; };
	quint32 getremotePort() { return m_remotePort; };
	bool getremote() { return m_remote; };
	bool getstartServer() { return m_startServer; };


private Q_SLOTS:
	void localPressed();
	void remotePressed();

private:

	QString m_serialPort;
	QString m_remoteHost;
	quint32 m_remotePort;
	bool m_remote;
	bool m_startServer;

	Ui::Startup *ui;
};

#endif // STARTUP_H
