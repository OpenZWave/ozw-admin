#ifndef OZWCORE_H
#define OZWCORE_H

#include <QObject>

class OZWCore : public QObject
{
    Q_OBJECT
public:
    explicit OZWCore(QObject *parent = nullptr);
    static OZWCore *get();
    void initilize();
signals:

public slots:
};



#endif // OZWCORE_H
