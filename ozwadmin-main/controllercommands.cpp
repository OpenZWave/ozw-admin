#include <QMessageBox>
#include "controllercommands.h"



ControllerCommands::ControllerCommands(QMainWindow *parent) : 
    QObject(parent)
{
 
}

void ControllerCommands::addNode() {
    QMessageBox::StandardButton ret = QMessageBox::question(qobject_cast<QMainWindow*>(this->parent()), "Include Secure?", "Do you wish to include the new device with encryption?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if (ret == QMessageBox::Cancel) {
        return;
    }
    this->m_msgBox = new QMessageBox(QMessageBox::Information, "Adding Node", "Starting....", QMessageBox::Cancel, qobject_cast<QMainWindow*>(this->parent()));
    this->m_msgBox->setDetailedText("Waiting for the Controller to Enter AddNode Mode...");
    connect(this->m_msgBox, &QMessageBox::rejected, this, &ControllerCommands::cancelCommand);
    this->m_msgBox->show();
    if (ret == QMessageBox::Yes) {
        OZWCore::get()->getQTOZWManager()->addNode(true);
    } else if (ret == QMessageBox::No) {
        OZWCore::get()->getQTOZWManager()->addNode(false);
    }
}
void ControllerCommands::delNode() {
    OZWCore::get()->getQTOZWManager()->removeNode();
}
void ControllerCommands::healNetwork() {
    QMessageBox::StandardButton ret = QMessageBox::information(qobject_cast<QMainWindow*>(this->parent()), "Heal Network", "Healing the Network Should only be performed after Adding/Removing or Physically Moving Mains Powered Devices. Are you sure?", QMessageBox::Ok|QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        OZWCore::get()->getQTOZWManager()->healNetwork(false);
    } 
}
void ControllerCommands::cancelCommand() {
    QMessageBox::StandardButton ret = QMessageBox::question(qobject_cast<QMainWindow*>(this->parent()), "Cancel Command", "Are you sure you wish to cancel the command?");
    if (ret == QMessageBox::Yes) {
        OZWCore::get()->getQTOZWManager()->cancelControllerCommand();
    }
}

