#include "mesh.h"
#include "QDebug"
#include "singletonrender.h"
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
    //<< nodesInMash[ID]->getName();
    return this->nodesInMash[ID];
}

int Mesh::generateId() {
    //qDebug() << "new Node with ID " << iDCounter << " added";
    return this->iDCounter++;
}

void Mesh::setFocusMeshObject(int nodeID) {
    this->focusObject = nodeID;
    if(nodeID > -1)
    {
        Node* n = nodesInMash[nodeID];
        QTableWidget *table = Data::instance()->getMainWindow()->ui->tableWidget;
        table->setRowCount(n->getParams()->size() + 3);

        //create the entries all nodes have
        QTableWidgetItem *name = new QTableWidgetItem(),
                *ID = new QTableWidgetItem(),
                *type = new QTableWidgetItem();

        //set the data
        ID->setData(0, nodeID);
        name->setData(0, n->getName());
        type->setData(0, n->getType());

        //asign to table
        table->setItem(0,0, new QTableWidgetItem("NodeID"));
        table->setItem(0,1, ID);
        table->item(0,0)->setFlags(table->item(0,0)->flags() ^ (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        table->setItem(1,0, new QTableWidgetItem("Node Class"));
        table->setItem(1,1, type);
        table->item(1,0)->setFlags(table->item(1,0)->flags() ^ (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        table->setItem(2,0, new QTableWidgetItem("Node Name"));
        table->setItem(2,1, name);

        //make name editable
        name->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);

        //avoid editing keys, and nodeID/type
        ID->setFlags(ID->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        type->setFlags(type->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        for(int i = 0; i < 3; i++)
            table->item(i,0)->setFlags(table->item(i,0)->flags()  ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
        QVariantMap *map = n->getParams();
        QStringList keys = map->keys();
        for(int i = 0; i < keys.size(); i++){
            //create key item
            QString key = keys.value(i);
            QTableWidgetItem *key_item = new QTableWidgetItem(key);
            key_item->setFlags(key_item->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
            table->setItem(3 + i,0,key_item);

            //create value item
            QVariant value = map->value(key);
            QTableWidgetItem *value_item;
            QSpinBox *spinner;
            switch(value.userType()){
            case QVariant::Bool:
                value_item = new QTableWidgetItem();
                //value_item->setData(0, value);
                value_item->setCheckState(value.toBool()? Qt::Checked : Qt::Unchecked);
                table->setItem(3 +i, 1, value_item);
                break;
            case QVariant::Int:
                spinner = new QSpinBox(table);
                spinner->setMaximum(std::numeric_limits<int>::max());
                spinner->setMinimum(std::numeric_limits<int>::min());
                spinner->setValue(value.toInt());
                table->setCellWidget(3 +i, 1, spinner);
                break;
            default:
                value_item = new QTableWidgetItem();
                value_item->setData(0, value);
                table->setItem(3 + i, 1, value_item);
                break;
            }

        }
        //TODO: generate TableContents dynamicly
        table->show();
    }
    else{
        QTableWidget* table = Data::instance()->getMainWindow()->ui->tableWidget;
        table->hide();
        //dele all tableitems, because they aren't needed any more
        for(int col = 0; col < table->columnCount(); col++)
            for(int row = 0; row < table->rowCount(); row++)
                delete table->item(row, col);
    }


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

int Mesh::getCurrentID(){ return iDCounter;}
