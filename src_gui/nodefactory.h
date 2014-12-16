#ifndef NODEFACTORY_H
#define NODEFACTORY_H
#include "node.h"
#include <QMap>

class NodeFactory
{
public:
    NodeFactory();
    static Node createNode();
    static Node createNode(const QString &_class);
    static Node createNode(const QString &_class, const QString &name);
    static Node createNode(const QString &_class, const QString &name, const QMap<QString,QString> &params);
};

#endif // NODEFACTORY_H
