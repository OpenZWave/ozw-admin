#include "dialog.h"
#include "ui_dialog.h"
#include <QScopedPointer>

void printSignal() {
    qDebug() << "Got Signal";
}


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QObject::connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &Dialog::startOZW);
    node.connectToNode(QUrl(QStringLiteral("local:openzwave")));
    node.setHeartbeatInterval(1000);
    model.reset(node.acquireModel(QStringLiteral("RemoteModel")));
    manager.reset(node.acquire<QTOZWManagerReplica>(QStringLiteral("Manager")));
    QObject::connect(manager.data(), &QTOZWManagerReplica::stateChanged, this, &Dialog::managerReady);
    manager->waitForSource(5000);
    ui->treeView->setModel(model.data());

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::managerReady(QRemoteObjectReplica::State state, QRemoteObjectReplica::State oldState) {
    if (state == QRemoteObjectReplica::Valid) {
        qDebug() << "State Valid" << state << " " << oldState;
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::valueAdded, &printSignal);
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::valueRemoved, &printSignal);
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::valueChanged, &printSignal);
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::valueRefreshed, &printSignal);
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::valuePollingEnabled, &printSignal);
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::valuePollingDisabled, &printSignal);
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::nodeGroupChanged, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeNew, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeAdded, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeRemoved, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeReset, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeNaming, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeEvent, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeProtocolInfo, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeEssentialNodeQueriesComplete, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::nodeQueriesComplete, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::driverReady, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::driverFailed, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::driverReset, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::driverRemoved, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::driverAllNodesQueriedSomeDead, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::driverAwakeNodesQueried, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::driverAllNodesQueried, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::controllerCommand, &printSignal);
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::ozwNotification, &printSignal);
        //     QObject::connect(manager.data(), &QTOZWManagerReplica::ozwUserAlert, &printSignal);
             QObject::connect(manager.data(), &QTOZWManagerReplica::manufacturerSpecificDBReady, &printSignal);

    } else {
        qDebug() << "State is not valid?" << state << " " << oldState;
    }
    Q_UNUSED(oldState);
}

void Dialog::startOZW() {
    manager->Start("/dev/ttyUSB0");

}
