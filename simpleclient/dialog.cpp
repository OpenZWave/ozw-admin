#include "dialog.h"
#include "ui_dialog.h"
#include <QScopedPointer>
#include "websocketiodevice.h"


void printSignal() {
    qDebug() << "Got Signal";
}



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QObject::connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &Dialog::startOZW);

    QRemoteObjectNode::RemoteObjectSchemaHandler setupTcp = [this](QUrl url) {
        qDebug() << "setupTcp" << url;
        WebSocketIODevice *socket = new WebSocketIODevice(this);
        connect(socket, &WebSocketIODevice::connected,
                [socket, this]() {
            qDebug() << "adding clientside connection " << socket;
            this->node.addClientSideConnection(socket);
        });
//      connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::error),
//        [socket](QAbstractSocket::SocketError error) {
//            delete socket;
//      });
        socket->setUrl(url);
        socket->open(QIODevice::ReadWrite);
    };
    //node.registerExternalSchema(QStringLiteral("ws"), setupTcp);



    node.connectToNode(QUrl(QStringLiteral("tcp://127.0.0.1:1983")));
    //setupTcp(QUrl(QStringLiteral("ws://localhost:1984")));
//    node.setHeartbeatInterval(2000);
    model.reset(node.acquireModel(QStringLiteral("nodeModel")));
    manager.reset(node.acquire<QTOZWManagerReplica>(QStringLiteral("Manager")));
    QObject::connect(manager.data(), &QTOZWManagerReplica::stateChanged, this, &Dialog::managerReady);
//    manager->waitForSource(5000);
    ui->treeView->setModel(model.data());
    qDebug() << node.lastError();




//    test.setUrl(QUrl("ws://localhost:1984/"));
//    //test.setProtocol("OpenZWave");

//    connect(&test, &WebSocketIODevice::socketConnected, this, [this]() {
//        qDebug() << "WebSocket connected, initializing MQTT connection.";
//    });
//    qDebug() << "trying";
//    test.open(QIODevice::ReadWrite);
//    qDebug() << "done";
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
      QRemoteObjectPendingReply<bool> result = manager->Start("/dev/cu.usbmodem14101");
      result.waitForFinished(30000);
}
