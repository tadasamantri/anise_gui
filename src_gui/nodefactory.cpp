#include "nodefactory.h"
#include "data.h"

// TODO friend Node, node should have a private konstruktor
// Nodes should only be created in this factory!
NodeFactory::NodeFactory() {}

Node NodeFactory::createNode() {
    Node node = Node();
    return node;
}

Node *NodeFactory::createNode(QString _class) {
    if(Data::instance()->getNodeCatalog()->contains(_class)){
        Node *node = new Node(Data::instance()->getNodeCatalog()->getNodeOfType(_class));
        return node;
    }
    return 0;
}

Node *NodeFactory::createNode(QString _class, QString name) {
    Node *node = createNode(_class);
    node->setName(name);
    return node;
}

Node *NodeFactory::createNode(QString _class, QString name, QMap<QString, Node::Parameter> params) {
    Node *node = createNode(_class, name);
    foreach (QString key, params.keys()) {

        node->addParam(key, params[key]);
    }
    return node;
}


Connection *NodeFactory::createConnection(int srcNodeID, QString srcGateName, int destNodeID, QString destGateName, QVector<QPoint> wayPoints){
    return new Connection(srcNodeID, srcGateName, destNodeID, destGateName, wayPoints);
}
