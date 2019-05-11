#ifndef QTOZWNOTIFICATION_H
#define QTOZWNOTIFICATION_H

#include <QObject>

class OZWNotification : public QObject
{
    Q_OBJECT
public:
    explicit OZWNotification(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QTOZWNOTIFICATION_H