#include "nodefactory.h"
#include "data.h"

// TODO friend Node, node should have a private konstruktor
// Nodes should only be created in this factory!
NodeFactory::NodeFactory() {}

/*Connection *NodeFactory::createTestConnection(int i){
    Connection *connection = new Connection();

    //create some random waypoints
    connection->waypoints.push_back(QPoint(0*i,0));
    connection->waypoints.push_back(QPoint(100*i,0));
    connection->waypoints.push_back(QPoint(0,100*i));
    connection->waypoints.push_back(QPoint(200*i,200*i));
    connection->waypoints.push_back(QPoint(300*i,300*i));

    return connection;
}*/


Node NodeFactory::createNode() {
    Node node = Node();
    return node;
}

Node *NodeFactory::createNode(QString _class) {
    Node *node = new Node(Data::instance()->getNodeCatalog()->getNodeOfType(_class));
    return node;
}

Node *NodeFactory::createNode(QString _class, QString name) {
    Node *node = createNode(_class);
    node->setName(name);
    return node;
}

Node *NodeFactory::createNode(QString _class, QString name, QMap<QString, Node::parameter> params) {
    Node *node = createNode(_class, name);
    foreach (QString key, params.keys()) {

        node->addParam(key, params[key]);
    }
    return node;
}


Connection *NodeFactory::createConnection(int srcNodeID, QString srcGateName, int destNodeID, QString destGateName, QVector<QPoint> wayPoints){


   // QPoint nodePosition = Data::instance()->getNodeByID(srcNodeID)->




    return new Connection(srcNodeID, srcGateName, destNodeID, destGateName, wayPoints);
    /*Connection *result = new Connection();
    Node *src = Data::instance()->getNodeByID(srcNodeID),
            *dst = Data::instance()->getNodeByID(destNodeID);
    Gate *srcGate = src->getGateByID(srcGateID, false),
            *destGate = dst->getGateByID(destGateID, true);
    result->setSrcNode(src);
    result->setDestNode(dst);
    result->setDestGate(destGate);
    result->setSrcGate(srcGate);
    result->setWaypoints(wayPoints);*/
    //return new Connection();

}
