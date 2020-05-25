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

public slots:
    void addNode();
    void delNode();
    void healNetwork();
    void cancelCommand();
private:
    QMessageBox *m_msgBox;
};

#endif // CONTROLLERCOMMANDS_H
