#ifndef QTOZWNODES_H
#define QTOZWNODES_H

#include <QObject>


#if defined(BUILD_REP)
#include "rep_OZWNodes_replica.h"
#else
#include "rep_OZWNodes_source.h"
#endif

#if defined(BUILD_REP)
class OZWNodes : public OZWNodesReplica
#else
class OZWNodes : public OZWNodesSimpleSource
#endif
{
public:
    explicit OZWNodes(QObject *parent = nullptr);
    explicit OZWNodes(const OZWNodes &) {}
    ~OZWNodes() {}
    bool operator!=(OZWNodes &other) {return true;}
    bool operator==(OZWNodes &other) {return true;}

//    OZWNodes operator=(OZWNodes &other) {return &other}

    friend QDataStream &operator<<(QDataStream &stream, const OZWNodes&);
    friend QDataStream &operator>>(QDataStream &, OZWNodes &);

signals:

public slots:
};

QDataStream &operator<<(QDataStream &stream, const OZWNodes&);
QDataStream &operator>>(QDataStream &, OZWNodes &);


Q_DECLARE_METATYPE(OZWNodes)

#endif // QTOZWNODES_H
