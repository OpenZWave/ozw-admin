#include "QTOZWNodes.h"
#if defined(BUILD_REP)
OZWNodes::OZWNodes(QObject *parent) : OZWNodesReplica (parent)
#else
OZWNodes::OZWNodes(QObject *parent) : OZWNodesSimpleSource (parent)
#endif
{

}



QDataStream &operator<<(QDataStream &stream, const OZWNodes *node)
{
    return stream;
}
QDataStream &operator>>(QDataStream &stream, OZWNodes *node)
{
    return stream;
}
