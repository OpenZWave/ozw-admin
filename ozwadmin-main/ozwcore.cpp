#include <QDebug>
#include "ozwcore.h"

Q_GLOBAL_STATIC(OZWCore, globalState)

OZWCore::OZWCore(QObject *parent) : QObject(parent)
{
    qDebug() << "Created";
}

OZWCore *OZWCore::get() {
    return globalState();
}


void OZWCore::initilize() {

}