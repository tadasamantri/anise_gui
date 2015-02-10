#include "mesh.h"
#include "data.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSpinBox>
#include <limits>

Mesh::Mesh(QObject *parent) : QObject(parent) {
    this->nodesInMash = QMap<int, Node *>();
    this->connectionsInMash = QMap<int, Connection *>();
    this->iDCounter = 0;
    this->focusObject = -1;
    tableExists = false;

}

int Mesh::addNode(Node *node) {
    int id = this->generateId();
    this->nodesInMash.insert(id, node);
    this->focusObject =
            id;  // A new created Node is always focussed in the beginning
    return id;
}

void Mesh::addNodes(QList<Node *> &list) {
    foreach (Node *node, list)
        this->addNode(node);
}

void Mesh::removeNode(int ID) {
    if (!nodesInMash.contains(ID)) return;
    Node *n = nodesInMash[ID];
    nodesInMash.remove(ID);
    delete n;
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
    foreach (Node *n, this->nodesInMash) {
        if (n->getName() == name) {
            node = n;
            break;
        }
    }
    return node;
}

Node *Mesh::getNodeByID(int ID) {
    // qDebug() << "getNodeByID returned Node with ID " << ID <<"\nName of Node is
    // "
    //<< nodesInMash[ID]->getName();
    return this->nodesInMash[ID];
}

Connection *Mesh::getConnectionByID(int ID){
    return this->connectionsInMash[ID];
}

int Mesh::generateId() {
    // qDebug() << "new Node with ID " << iDCounter << " added";
    return this->iDCounter++;
}

void Mesh::setFocusMeshObject(int nodeID) {
    this->focusObject = nodeID;
    Data::instance()->getMainWindow()->updatePropertyTable(nodeID);
}

bool Mesh::deleteItem() {
    if (this->focusObject == -1) return false;

    if (nodesInMash.contains(focusObject)) return this->deleteNode();
    if (connectionsInMash.contains((focusObject)))
        return this->deleteConnection();
    return false;
}

bool Mesh::deleteNode() {
    nodesInMash.remove(focusObject);
    Data::instance()->getMainWindow()->updatePropertyTable(-1);
    bool allRemoved = !nodesInMash.contains(focusObject) &&
            SingletonRender::instance()->deleteMeshDrawing(focusObject);

    if (allRemoved) focusObject = -1;

    return allRemoved;
}

bool Mesh::deleteConnection() {
    connectionsInMash.remove(focusObject);
    bool allRemoved = connectionsInMash.contains(focusObject) &&
            SingletonRender::instance()->deleteMeshDrawing(focusObject);

    if (allRemoved) focusObject = -1;

    return allRemoved;
}

int Mesh::getCurrentID() { return iDCounter; }
