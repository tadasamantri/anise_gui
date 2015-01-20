#include "nodefactory.h"
//TODO friend Node, node should have a private konstruktor
NodeFactory::NodeFactory() {}
Node NodeFactory::createNode() {
  Node node = Node();
  return node;
}

Node NodeFactory::createNode(QString _class) {
  Node node = createNode();
  node.setType(_class);
  return node;
}

Node NodeFactory::createNode(QString _class, QString name) {
  Node node = createNode(_class);
  node.setName(name);
  return node;
}

Node NodeFactory::createNode(QString _class, QString name, QVariantMap params) {
  Node node = createNode(_class, name);
  foreach (QString key, params.keys()) {
    node.addParam(key, params.find(key).value());
  }
  return node;
}

Node *NodeFactory::createTestNode() {
  // ##### generate a node just for testing purpose
  // later nodes should be created by reading from a json file

  Node *tempTestNode = new Node(); // using standart construktor
  // setting individual variables

  // name and type
  tempTestNode->setType("TestType");
  tempTestNode->setName("Dieter");

  // generate and add test gates:
  // input
  QVector<QString> *tempTestGateInputTypes = new QVector<QString>();
  tempTestGateInputTypes->append("type1");
  tempTestGateInputTypes->append("type2");
  tempTestGateInputTypes->append("type3");
  // output
  QVector<QString> *tempTestGateOutputTypes = new QVector<QString>();
  tempTestGateInputTypes->append("type4");
  tempTestGateInputTypes->append("type1");
  tempTestGateInputTypes->append("type5");

  Gate *tempTestGateInput = new Gate(true, *tempTestGateInputTypes);
  Gate *tempTestGateOutput = new Gate(false, *tempTestGateOutputTypes);
  tempTestNode->addGate(*tempTestGateInput, 1);
  tempTestNode->addGate(*tempTestGateOutput, 0);

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
