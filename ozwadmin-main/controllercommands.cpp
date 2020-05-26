#include <QMessageBox>
#include "controllercommands.h"
#include "util.h"



ControllerCommands::ControllerCommands(QMainWindow *parent) : 
    QObject(parent),
    m_msgBox(parent)
{
    connect(OZWCore::get()->getQTOZWManager(), &QTOZWManager::controllerCommand, this, &ControllerCommands::controllerCommandNotification);
}


void ControllerCommands::addNode() {


    this->m_command = ControllerCommands::Command_addNode;
    QMessageBox::StandardButton ret = QMessageBox::question(qobject_cast<QMainWindow*>(this->parent()), "Include Secure?", "Do you wish to include the new device with encryption?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if (ret == QMessageBox::Cancel) {
        return;
    }
    this->m_msgBox.setIcon(QMessageBox::Information);
    this->m_msgBox.setWindowTitle("Adding Node");
    this->m_msgBox.setText("Starting...");
    this->m_msgBox.setStandardButtons(QMessageBox::Cancel);
    this->m_msgBox.setDetailedText("Waiting for the Controller to Enter AddNode Mode...");
    connect(&this->m_msgBox, &QMessageBox::rejected, this, &ControllerCommands::cancelCommand);
    this->m_msgBox.show();
    if (ret == QMessageBox::Yes) {
        OZWCore::get()->getQTOZWManager()->addNode(true);
    } else if (ret == QMessageBox::No) {
        OZWCore::get()->getQTOZWManager()->addNode(false);
    }
}
void ControllerCommands::delNode() {
    this->m_command = ControllerCommands::Command_delNode;
    this->m_msgBox.setIcon(QMessageBox::Information);
    this->m_msgBox.setWindowTitle("Exclude Node");
    this->m_msgBox.setText("Starting...");
    this->m_msgBox.setStandardButtons(QMessageBox::Cancel);
    this->m_msgBox.setDetailedText("Waiting for the Controller to Enter Exclusion Mode...");
    connect(&this->m_msgBox, &QMessageBox::rejected, this, &ControllerCommands::cancelCommand);
    this->m_msgBox.show();
    OZWCore::get()->getQTOZWManager()->removeNode();
}
void ControllerCommands::healNetwork() {
    this->m_command = ControllerCommands::Command_healNetwork;
    QMessageBox::StandardButton ret = QMessageBox::information(qobject_cast<QMainWindow*>(this->parent()), "Heal Network", "Healing the Network Should only be performed after Adding/Removing or Physically Moving Mains Powered Devices. Are you sure?", QMessageBox::Ok|QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        this->m_msgBox.setIcon(QMessageBox::Information);
        this->m_msgBox.setWindowTitle("Heal Network");
        this->m_msgBox.setText("Starting...");
        this->m_msgBox.setStandardButtons(QMessageBox::Cancel);
        this->m_msgBox.setDetailedText("Waiting for the Controller to Enter Heal Network Mode...");
        connect(&this->m_msgBox, &QMessageBox::rejected, this, &ControllerCommands::cancelCommand);
        this->m_msgBox.show();
        OZWCore::get()->getQTOZWManager()->healNetwork(false);
    } 
}
void ControllerCommands::cancelCommand() {
    switch (this->m_command) {
        case ControllerCommands::Command_none:
        case ControllerCommands::Command_healNetwork:
        case ControllerCommands::Command_cancel:
        {
            /* These Don't need to be Canceled */
            break;
        }
        default:
        {
            QMessageBox::StandardButton ret = QMessageBox::question(qobject_cast<QMainWindow*>(this->parent()), "Cancel Command", "Are you sure you wish to cancel the command?");
            if (ret == QMessageBox::Yes) {
                OZWCore::get()->getQTOZWManager()->cancelControllerCommand();
            } else {
                this->m_msgBox.show();
            }
            break;
        }

    }
}

void ControllerCommands::controllerCommandNotification(quint8 node, NotificationTypes::QTOZW_Notification_Controller_Cmd command, NotificationTypes::QTOZW_Notification_Controller_State state, NotificationTypes::QTOZW_Notification_Controller_Error error)
{
    qCDebug(ozwadmin) << "ControllerCommandNotification: " << node << command << state << error;
    switch (this->m_command) {
        case ControllerCommands::Command_none: 
        case ControllerCommands::Command_cancel:
        {
            qCDebug(ozwadmin) << "Ignoring Notification as we are not in a ControllerCommand Mode";
            return;
        }
        case ControllerCommands::Command_addNode: {
            if (command != NotificationTypes::Ctrl_Cmd_AddNode) {
                qCDebug(ozwadmin) << "Ignoring Notification as we are not in addNode Mode";
                return;
            }
            switch (state) {
                case NotificationTypes::Ctrl_State_Starting: {
                    this->m_msgBox.setText("Add Node Starting...");
                    break;
                }
                case NotificationTypes::Ctrl_State_Waiting: {
                    this->m_msgBox.setText("Started - Waiting to discover new Node");
                    this->m_msgBox.setDetailedText("Please Activate the Pairing Mode on the device you wish to include now");
                    break;
                }
                case NotificationTypes::Ctrl_State_Normal: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Cancel: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Error: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Sleeping: {
                    break;
                }
                case NotificationTypes::Ctrl_State_InProgress: {
                    this->m_msgBox.setIcon(QMessageBox::Information);
                    if (node) {
                        this->m_msgBox.setText(QString("Adding Node %1 in Progress").arg(node));
                    } else {
                        this->m_msgBox.setText("Adding Node in Progress");
                    }
                    this->m_msgBox.setDetailedText("OpenZWave is Querying Node Capabilities...");
                    this->m_msgBox.setStandardButtons(QMessageBox::NoButton);
                    break;
                }
                case NotificationTypes::Ctrl_State_Completed: {
                    this->m_msgBox.setIcon(QMessageBox::Information);
                    this->m_msgBox.setText(QString("Add Node Completed for new Node %1").arg(node));
                    this->m_msgBox.setDetailedText("Add Node Has Completed. OpenZWave is querying the the Node now");
                    this->m_msgBox.setStandardButtons(QMessageBox::Close);
                    this->m_command = ControllerCommands::Command_none;
                    break;
                }
                case NotificationTypes::Ctrl_State_Failed: {
                    this->m_msgBox.setIcon(QMessageBox::Critical);
                    this->m_msgBox.setText("AddNode Command Failed");
                    this->m_msgBox.setDetailedText("Please Consult the Logs Window for further infomation");
                    this->m_msgBox.setStandardButtons(QMessageBox::Close);
                    break;
                }
                case NotificationTypes::Ctrl_State_NodeOk: {
                    break;
                }
                case NotificationTypes::Ctrl_State_NodeFailed: {
                    break;
                }
                case NotificationTypes::Ctrl_State_count: {
                    break;
                }
            }
            break;            
        }
        case ControllerCommands::Command_delNode: {
            if (command != NotificationTypes::Ctrl_Cmd_RemoveNode) {
                qCDebug(ozwadmin) << "Ignoring Notification as we are not in delNode Mode";
                return;
            }
            switch (state) {
                case NotificationTypes::Ctrl_State_Starting: {
                    this->m_msgBox.setText("Delete Node Starting...");
                    break;
                }
                case NotificationTypes::Ctrl_State_Waiting: {
                    this->m_msgBox.setText("Delete Node Mode Started - Waiting...");
                    this->m_msgBox.setDetailedText("Please activate the exclude mode function on the device you wish to exclude now");
                    break;
                }
                case NotificationTypes::Ctrl_State_Normal: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Cancel: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Error: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Sleeping: {
                    break;
                }
                case NotificationTypes::Ctrl_State_InProgress: {
                    this->m_msgBox.setIcon(QMessageBox::Information);
                    if (node) {
                        this->m_msgBox.setText(QString("Exectuting Exclusion on Node %1").arg(node));
                    } else { 
                        this->m_msgBox.setText("Exectuting Exclusion on Device");
                    }
                    this->m_msgBox.setDetailedText("Node is currently being removed from its Network Association");
                    this->m_msgBox.setStandardButtons(QMessageBox::NoButton);
                    break;
                }
                case NotificationTypes::Ctrl_State_Completed: {
                    this->m_msgBox.setIcon(QMessageBox::Information);
                    if (node) {
                        this->m_msgBox.setText(QString("Delete Node Completed for Node %1").arg(node));
                    } else { 
                        this->m_msgBox.setText(QString("Delete Node Completed").arg(node));
                    }
                    this->m_msgBox.setDetailedText("Delete Node Has Completed");
                    this->m_msgBox.setStandardButtons(QMessageBox::Close);
                    this->m_command = ControllerCommands::Command_none;
                    break;
                }
                case NotificationTypes::Ctrl_State_Failed: {
                    this->m_msgBox.setIcon(QMessageBox::Warning);
                    this->m_msgBox.setText("Delete Node Command Failed");
                    this->m_msgBox.setDetailedText("If you were excluding a device associated from another Network, the Node may have been removed from that network and is ready to be included in a new Network");
                    this->m_msgBox.setStandardButtons(QMessageBox::Close);
                    break;
                }
                case NotificationTypes::Ctrl_State_NodeOk: {
                    break;
                }
                case NotificationTypes::Ctrl_State_NodeFailed: {
                    break;
                }
                case NotificationTypes::Ctrl_State_count: {
                    break;
                }
            }   
            break;         
        }
        case ControllerCommands::Command_healNetwork: {
            static int sleeping = 0;
            static int failed = 0;
            if (command != NotificationTypes::Ctrl_Cmd_RequestNodeNeighborUpdate) {
                qCDebug(ozwadmin) << "Ignoring Notification as we are not in healNetwork Mode";
                return;
            }
            switch (state) {
                case NotificationTypes::Ctrl_State_Starting: {
                    sleeping = 0;
                    failed = 0;
                    this->m_msgBox.setText("Heal Network Mode Starting...");
                    break;
                }
                case NotificationTypes::Ctrl_State_Waiting: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Normal: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Cancel: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Error: {
                    break;
                }
                case NotificationTypes::Ctrl_State_Sleeping: {
                    sleeping++;
                    QString details;
                    if (sleeping > 0) {
                        this->m_msgBox.setIcon(QMessageBox::Warning);
                        details.append("Some Devices are Sleeping\n");
                    }
                    if (failed > 0) {
                        this->m_msgBox.setIcon(QMessageBox::Critical);
                        details.append("Some Devices Failed\n");
                    }

                    this->m_msgBox.setIcon(QMessageBox::Warning);
                    this->m_msgBox.setDetailedText(details);
                    this->m_msgBox.setStandardButtons(QMessageBox::Close);
                    break;
                }
                case NotificationTypes::Ctrl_State_InProgress: {
                    QString details;
                    if (sleeping > 0) {
                        this->m_msgBox.setIcon(QMessageBox::Warning);
                        details.append("Some Devices are Sleeping\n");
                    }
                    if (failed > 0) {
                        this->m_msgBox.setIcon(QMessageBox::Critical);
                        details.append("Some Devices Failed\n");
                    }
                    if (node) {
                        this->m_msgBox.setText(QString("Exectuting Heal Network on Node %1").arg(node));
                    } else { 
                        this->m_msgBox.setText("Exectuting Heal Network");
                    }
                    if (!details.isEmpty()) this->m_msgBox.setDetailedText(details);
                    this->m_msgBox.setStandardButtons(QMessageBox::Close);
                    break;
                }
                case NotificationTypes::Ctrl_State_Completed: {
                    QString details;
                    if (sleeping > 0) {
                        this->m_msgBox.setIcon(QMessageBox::Warning);
                        details.append("Some Devices are Sleeping\n");
                    }
                    if (failed > 0) {
                        this->m_msgBox.setIcon(QMessageBox::Critical);
                        details.append("Some Devices Failed\n");
                    }
                    this->m_msgBox.setText("Heal Network Completed");
                    this->m_msgBox.setDetailedText(details);
                    this->m_msgBox.setStandardButtons(QMessageBox::Close);
                    this->m_command = ControllerCommands::Command_none;
                    break;
                }
                case NotificationTypes::Ctrl_State_Failed: {
                    if (node == OZWCore::get()->getQTOZWManager()->getControllerNodeId()) {
                        /* controller Nodes always fail this */
                        return;
                    }
                    failed++;
                    QString details;
                    if (sleeping > 0) {
                        this->m_msgBox.setIcon(QMessageBox::Warning);
                        details.append("Some Devices are Sleeping\n");
                    }
                    if (failed > 0) {
                        this->m_msgBox.setIcon(QMessageBox::Critical);
                        details.append("Some Devices Failed\n");
                    }
                    this->m_msgBox.setText("Heal Network Finished");
                    this->m_msgBox.setDetailedText(details);
                    this->m_msgBox.setStandardButtons(QMessageBox::Close);
                    break;
                }
                case NotificationTypes::Ctrl_State_NodeOk: {
                    break;
                }
                case NotificationTypes::Ctrl_State_NodeFailed: {
                    break;
                }
                case NotificationTypes::Ctrl_State_count: {
                    break;
                }
            }            
        }

    }
}
