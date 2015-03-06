#include "mesh.h"
#include "data.h"
#include "math.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSpinBox>
#include <limits>

Mesh::Mesh(QObject *parent) : QObject(parent) {
    this->nodesInMesh = QMap<int, Node *>();
    this->connectionsInMesh = QMap<int, Connection *>();
    this->iDCounter = 0;
    this->focusObject = -1;
    tableExists = false;
}
QList<Connection*> Mesh::getConnectionsToNode(int nodeID){
    QList<Connection*> list;
    for(Connection *c : connectionsInMesh){
        if(c->getSrcNode()->getID() == nodeID || c->getDestNode()->getID() == nodeID)
            list << c;
    }
    return list;
}

int Mesh::addNode(Node *node) {
    node->setName(getValidAlternativeForName(node->getName()));
    int id = generateId();
    node->setID(id);

    this->nodesInMesh.insert(id, node);

    return id;
}

int Mesh::addNodes(QList<Node *> &list) {
    int id = -1;
    foreach (Node *node, list)
        id = addNode(node);
    return id;
}

void Mesh::removeNode(int ID) {
    if (!nodesInMesh.contains(ID)) return;
    Node *n = nodesInMesh[ID];
    nodesInMesh.remove(ID);
    delete n;
}

int Mesh::addConnection(Connection *connection) {
    int id = this->generateId();
    connection->setID(id);
    this->connectionsInMesh.insert(id, connection);
    return id;
}

QList<Node *> Mesh::getAllNodes() { return this->nodesInMesh.values(); }

QList<Connection *> Mesh::getAllConnections() {
    return this->connectionsInMesh.values();
}

Node *Mesh::getFocusedNode() {
    if (focusObject == -1) return 0;

    return getNodeByID(focusObject);
}

Node *Mesh::getNodeByName(QString name) {
    // TODO probably easy to optimize
    Node *node = 0;
    foreach (Node *n, this->nodesInMesh) {
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
    return this->nodesInMesh.value(ID, 0);
}

Connection *Mesh::getConnectionByID(int ID) {
    return this->connectionsInMesh[ID];
}

int Mesh::generateId() {
    // qDebug() << "new Node with ID " << iDCounter << " added";
    return this->iDCounter++;
}

void Mesh::setFocusMeshObject(int nodeID) {

    if(this->focusObject != -1)
        SingletonRender::instance()->dehighlightObject(this->focusObject);

    this->focusObject = nodeID;
    SingletonRender::instance()->highlightObject(this->focusObject);

    Data::instance()->getMainWindow()->updatePropertyTable(nodeID);
}
/**
 * @brief Mesh::updateNode
 * SLOT: applies property changes made in tableview to nodes
 * @param item
 */
void Mesh::updateNode(QTableWidgetItem *item) {
    if (item->column() == 0 || item->column() > 1) return;
    QTableWidget *table = Data::instance()->getMainWindow()->ui->tableWidget;
    int row = item->row();
    Node *n = getFocusedNode();
    QTableWidgetItem *theItemID = table->item(row, 0);
    if (!theItemID) return;
    QString paramID = theItemID->text();
    if (paramID == "Node Name"){
        QString name = getValidAlternativeForName(item->text());
        n->setName(name);
    }
    else {
        paramID = theItemID->data(Qt::UserRole).toString();
        QVariant _old = n->getParamByKey(paramID), _new;
        QVariant data = item->text();

        switch (_old.type()) {
        case QVariant::UInt:
            _new = data.toUInt();
            break;
        case QVariant::Int:
            _new = data.toInt();
            break;
        case QVariant::Bool:
            _new = item->checkState() == Qt::Checked;
            break;
        default:
            _new = data;
        }
        n->setParam(paramID, _new);
        item->setData(Qt::UserRole, _new);
    }
    Data::instance()->getMainWindow()->updatePropertyTable(focusObject);
}

bool Mesh::deleteItem() {
    if (this->focusObject == -1) return false;

    if (nodesInMesh.contains(focusObject))
        return this->deleteNode();
    if (connectionsInMesh.contains((focusObject)))
        return this->deleteConnection();
    return false;
}

bool Mesh::validName(const QString &name) {
    if (name == "") return false;

    foreach (Node *n, nodesInMesh.values())
        if (n->getName() == name) return false;

    return true;
}

QString Mesh::getValidAlternativeForName(const QString name){
    if(validName(name))
        return name;
    QString tmpName = name;
    int i = 0;
    while(!validName(tmpName))
        tmpName = name + QString::number(i++);
    return tmpName;
}

bool Mesh::deleteNode() {

    //Get Pointer for deletingprocess later
    Node *nodeToDelete = nodesInMesh.value(focusObject);
    //remove Node from our Map
    nodesInMesh.remove(focusObject);

    //delete drawing and check that everything ist deleted
    bool allRemoved = !nodesInMesh.contains(focusObject) &&
            SingletonRender::instance()->deleteMeshDrawing(focusObject);

    //If Node was deleted correctly...
    if (allRemoved){

        //... all connections attached should be deleted too
        foreach(Connection *c , connectionsInMesh){

            if(c->getSrcNode()->getID() == focusObject || c->getDestNode()->getID() == focusObject){
                this->deleteConnection(c);
            }
        }

        //...and nothing is highlighted anymore
        this->setFocusMeshObject(-1);

        //...and Node has to get deleted too
        delete nodeToDelete;
    }

    //Update Property Table
    Data::instance()->getMainWindow()->updatePropertyTable(-1);

    return allRemoved;
}

bool Mesh::deleteConnection(Connection *c){

    return deleteConnection(c->getID());

}

bool Mesh::deleteConnection(int conToDeleteID){

    connectionsInMesh.remove(conToDeleteID);
    bool allRemoved = !connectionsInMesh.contains(conToDeleteID) &&
            SingletonRender::instance()->deleteMeshDrawing(conToDeleteID);

    if(this->focusObject == conToDeleteID)
        this->setFocusMeshObject(-1);

    return allRemoved;

}

bool Mesh::deleteConnection() {

    if (deleteConnection(focusObject)) {
        this->setFocusMeshObject(-1);
        return true;
    }
    return false;
}

int Mesh::getCurrentID() { return iDCounter; }


void Mesh::updateConnStartAndEnd(){
}

void Mesh::sort(){

    //this will sort any mesh depending on the connections

    //first remove all waypoints of each connection, later the renderclass will add new waypoints if none are found
    foreach (Connection *c, this->getAllConnections()) {
        c->waypoints.clear();
    }

    //now sort all nodes in a circle
    int numberOfNodes = this->getAllNodes().size();

    double degreeDistance = 2.0*M_PI/float(numberOfNodes);

    double nodeDistance = 80; //distance to the next node; higher value make the circle bigger

    double radius = (nodeDistance * numberOfNodes)/(2.0*M_PI); //M_PI = pi

    double angle = 0;

    int posx, posy;

    foreach (Node *n, this->getAllNodes()) {


        posx = int(radius * cos(angle) + radius);
        qDebug() << "cos " << cos(angle) << " sin " << sin(angle) << " degree: " << angle;
        posy = int(radius * sin(angle) + radius);

        n->setPosition(posx, posy);
        angle += degreeDistance;
    }


}

Mesh::~Mesh(){
    for(Connection *c : connectionsInMesh)
        delete c;
    for(Node *n : nodesInMesh)
        delete n;
}
