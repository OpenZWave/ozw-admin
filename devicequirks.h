#ifndef DEVICEQUIRKS_H
#define DEVICEQUIRKS_H

#include <QObject>

class DeviceQuirks : public QObject
{
    Q_OBJECT
public:
    explicit DeviceQuirks(QObject *parent = 0);

signals:

public slots:
};

#endif // DEVICEQUIRKS_H