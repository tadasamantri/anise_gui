#ifndef NODEFACTORY_H
#define NODEFACTORY_H
#include "node.h"
#include <QVariantMap>

class NodeFactory {
public:
    NodeFactory();
    // static damit man kein objekt davon erstellen muss.
    static Node createNode();
    static Node createNode(QString _class);
    static Node createNode(QString _class, QString name);
    static Node createNode(QString _class, QString name, QVariantMap params);
    static Node *createTestNode();
};

#endif  // NODEFACTORY_H
