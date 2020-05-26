#ifndef CONTROLLERCOMMANDS_H
#define CONTROLLERCOMMANDS_H

#include <QObject>
#include <QMainWindow>
#include "ozwcore.h"

class OZWCore;

class ControllerCommands : public QObject
{
    Q_OBJECT
public:
    explicit ControllerCommands(QMainWindow *parent = nullptr);
    enum Command {Command_addNode, Command_delNode, Command_healNetwork, Command_cancel, Command_none};
    Q_ENUM(Command);
public slots:
    void addNode();
    void delNode();
    void healNetwork();
    void cancelCommand();

    void controllerCommandNotification(quint8 node, NotificationTypes::QTOZW_Notification_Controller_Cmd command, NotificationTypes::QTOZW_Notification_Controller_State state, NotificationTypes::QTOZW_Notification_Controller_Error error);
private:
    QMessageBox m_msgBox;
    Command m_command;
};

#endif // CONTROLLERCOMMANDS_H
