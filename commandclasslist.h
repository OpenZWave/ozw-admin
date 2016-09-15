#ifndef COMMANDCLASSLIST_H
#define COMMANDCLASSLIST_H

#include <QMap>

class CommandClassList
{
public:
    CommandClassList();
    QString getName(int cc);
    int getID(QString name);
private:
    QMap<int, QString> cclist;
};

#endif // COMMANDCLASSLIST_H
