#ifndef DEVICEQUIRKS_H
#define DEVICEQUIRKS_H

#include <QObject>
#include <QDomDocument>
#include <QHash>

class DeviceQuirks : public QObject
{
    Q_OBJECT
public:
    static DeviceQuirks &GetInstance();
    bool isReady() const;
    void dump();
private:
    struct QuirksEntry {
        QList<QString> options;
        QList<uint8_t> CommandClasses;
        QString Help;
    };

    explicit DeviceQuirks(QObject *parent = 0);
    void setReady(bool ready);
signals:

public slots:

private:
    QDomDocument domDocument;
    bool m_Ready;
    QHash<QString, QuirksEntry*> m_quirks;
};

#endif // DEVICEQUIRKS_H
