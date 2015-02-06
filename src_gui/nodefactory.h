#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include "node.h"
#include <QVariantMap>
#include <QDebug>
#include "nodecatalog.h"
#include "connection.h"

class NodeFactory {
public:

    static Node createNode();
    static Node *createNode(QString _class);
    static Node *createNode(QString _class, QString name);
    static Node *createNode(QString _class, QString name, QVariantMap params);
    static Node *createTestNode();

    static Connection *createTestConnection(int i);
    static Connection *createConnection(int srcNodeID, int srcGateID, int destNodeID, int destGateID, QVector<QPoint> wayPoints);

    NodeFactory();

};

#endif  // NODEFACTORY_H
