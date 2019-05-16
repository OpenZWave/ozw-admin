#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QObject>
#include <QRemoteObjectNode>
#include <QAbstractItemModelReplica>
#include "rep_qtozwmanager_replica.h"
#include "websocketiodevice.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public Q_SLOTS:
    void startOZW();
    void managerReady(QRemoteObjectReplica::State state, QRemoteObjectReplica::State oldState);

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();


private:
    Ui::Dialog *ui;
    QRemoteObjectNode node;
    QScopedPointer<QAbstractItemModelReplica> model;
    QScopedPointer<QAbstractItemModelReplica> valuemodel;
    QScopedPointer<QAbstractItemModelReplica> assocmodel;

    QScopedPointer<QTOZWManagerReplica> manager;
    WebSocketIODevice test;
};

#endif // DIALOG_H
