#include "nodefactory.h"
#include "data.h"

// TODO friend Node, node should have a private konstruktor
// Nodes should only be created in this factory!
NodeFactory::NodeFactory() {}

Connection *NodeFactory::createTestConnection(int i){
    Connection *connection = new Connection();

    //create some random waypoints
    connection->waypoints.push_back(QPoint(0*i,0));
    connection->waypoints.push_back(QPoint(100*i,0));
    connection->waypoints.push_back(QPoint(0,100*i));
    connection->waypoints.push_back(QPoint(200*i,200*i));
    connection->waypoints.push_back(QPoint(300*i,300*i));

    return connection;
}


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

Node *NodeFactory::createNode(QString _class, QString name, QVariantMap params) {
    Node *node = createNode(_class, name);
    foreach (QString key, params.keys()) {

        node->addParam(key, params[key]);
    }
    return node;
}

Node *NodeFactory::createTestNode() {
    // ##### generate a node just for testing purpose
    // later nodes should be created by reading from a json file

    Node *tempTestNode = new Node();  // using standart construktor
    // setting individual variables

    // name and type
    tempTestNode->setType("TestType");
    tempTestNode->setName("Dieter");

    // generate and add test gates:
    // input
    QList<QString> *tempTestGateInputTypes = new QList<QString>();
    tempTestGateInputTypes->append("type1");
    tempTestGateInputTypes->append("type2");
    tempTestGateInputTypes->append("type3");
    // output
    QList<QString> *tempTestGateOutputTypes = new QList<QString>();
    tempTestGateInputTypes->append("type4");
    tempTestGateInputTypes->append("type1");
    tempTestGateInputTypes->append("type5");

    Gate *tempTestGateInput = new Gate(true, *tempTestGateInputTypes);
    Gate *tempTestGateOutput = new Gate(false, *tempTestGateOutputTypes);
    tempTestNode->addGate(tempTestGateInput);
    tempTestNode->addGate(tempTestGateOutput);

    // add parameter
    tempTestNode->addParam("param1", 1);
    tempTestNode->addParam("param2", false);
    tempTestNode->addParam("param3", 19.4f);
    tempTestNode->addParam("param4", 3.141);
    tempTestNode->addParam("param5", "Wurst");

    // set the position of the Node
    tempTestNode->setPosition(1.0, 1.0);

    // ##### end of generation of testing node

    return tempTestNode;
}
