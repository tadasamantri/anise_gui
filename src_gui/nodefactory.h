#ifndef NODEFACTORY_H
#define NODEFACTORY_H
#include "node.h"
#include <QVariantMap>

class NodeFactory
{
public:
    NodeFactory();
    static Node createNode();
    static Node createNode(QString _class);
    static Node createNode(QString _class,  QString name);
    static Node createNode(QString _class, QString name, QVariantMap params);
};

#endif // NODEFACTORY_H
