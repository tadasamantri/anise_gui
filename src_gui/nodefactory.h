#ifndef NODEFACTORY_H
#define NODEFACTORY_H
#include "node.h"
#include <QVariantMap>
#include <QDebug>
#include "nodecatalog.h"


class NodeFactory { //singleton
public:

    static Node createNode();
    static Node *createNode(QString _class);
    static Node *createNode(QString _class, QString name);
    static Node *createNode(QString _class, QString name, QVariantMap params);
    static Node *createTestNode();


    NodeFactory();

};

#endif  // NODEFACTORY_H
