#include "mesh.h"
#include "QDebug"

Mesh::Mesh() {
  this->nodesInMash = QMap<int, Node *>();
  this->connectionsInMash = QMap<int, Connection *>();
}

int Mesh::addNode(Node *node) {
  int id = this->generateId();
  this->nodesInMash.insert(id, node);
  return id;
}

int Mesh::addConnection(Connection *connection) {
  int id = this->generateId();
  this->connectionsInMash.insert(id, connection);
  return id;
}

QList<Node *> Mesh::getAllNodes() { return this->nodesInMash.values(); }

QList<Connection *> Mesh::getAllConnections() {
  return this->connectionsInMash.values();
}

Node *Mesh::getNodeByName(QString name) {
  // TODO probably easy to optimize
  Node *node;
  foreach (Node *n, this->nodesInMash.values()) {
    if (n->name == name) {
      node = n;
    }
  }
  return node;
}

Node *Mesh::getNodeByID(int ID) { return this->nodesInMash.value(ID); }

int Mesh::generateId() {
  iDCounter++;
  return this->iDCounter;
}
