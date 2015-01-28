#include "mesh.h"
#include "QDebug"
#include "singletonrender.h"

Mesh::Mesh(QObject *parent) : QObject(parent) {
    this->nodesInMash = QMap<int, Node *>();
    this->connectionsInMash = QMap<int, Connection *>();
    this->iDCounter = 0;
    this->focusObject = -1;
}

int Mesh::addNode(Node *node) {
    int id = this->generateId();
    this->nodesInMash.insert(id, node);
    this->focusObject = id;             //A new created Node is always focussed in the beginning
    return id;

}

void Mesh::addNodes(QList<Node *> &list) {
    foreach (Node *node, list)
        this->addNode(node);
}

void Mesh::removeNode(int ID)
{
    if(!nodesInMash.contains(ID))
        return;
    Node* n = nodesInMash[ID];
    nodesInMash.remove(ID);
    delete n;
}

void Mesh::addConnections(QList<Connection *> &list) {
    foreach (Connection *connection, list)
        this->addConnection(connection);
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
        if (n->name == name) {
            node = n;
            break;
        }
    }
    return node;
}

Node *Mesh::getNodeByID(int ID) {
    //qDebug() << "getNodeByID returned Node with ID " << ID <<"\nName of Node is "
                << nodesInMash[ID]->getName();
    return this->nodesInMash[ID];
}

int Mesh::generateId() {
    //qDebug() << "new Node with ID " << iDCounter << " added";
    return this->iDCounter++;
}

void Mesh::setFocusMeshObject(int nodeID) {
    this->focusObject = nodeID;

  //qDebug() << "I GOT CKLICKED. MY ID: " << nodeID;
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
