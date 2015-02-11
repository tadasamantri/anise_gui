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
    node->setName(getValidAlternativeForName(node->getName()));
    int id = generateId();
    this->nodesInMash.insert(id, node);
    this->focusObject =
            id;  // A new created Node is always focussed in the beginning
    return id;
}

int Mesh::addNodes(QList<Node *> &list) {
    int id = -1;
    foreach (Node *node, list)
        id = addNode(node);
    return id;
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

Node *Mesh::getFocusedNode() {
    if (focusObject == -1) return 0;

    return getNodeByID(focusObject);
}

Node *Mesh::getNodeByName(QString name) {
    // TODO probably easy to optimize
    Node *node = 0;
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

Connection *Mesh::getConnectionByID(int ID) {
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
        if (!_new.type() == QVariant::Bool) item->setText(_new.toString());
    }
    Data::instance()->getMainWindow()->updatePropertyTable(focusObject);
}

bool Mesh::deleteItem() {
    if (this->focusObject == -1) return false;

    if (nodesInMash.contains(focusObject)) return this->deleteNode();
    if (connectionsInMash.contains((focusObject)))
        return this->deleteConnection();
    return false;
}

bool Mesh::validName(const QString &name) {
    if (name == "") return false;

    foreach (Node *n, nodesInMash.values())
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
