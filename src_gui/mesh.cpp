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

bool Mesh::checkConnection(int srcNodeID, QString srcGate, int destNodeID, QString destGate){
    Node *srcN = nodesInMesh[srcNodeID], *destN = nodesInMesh[destNodeID];
    if(!(srcN && destN))
        return false;
    Gate *srcG = srcN->getGateByName(srcGate), *destG = destN->getGateByName(destGate);
    if(!(srcG && destG))
        return false;
    for(QString t : srcG->getTypes())
        if(destG->hasType(t))
            return true;
    return false;
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

void Mesh::sortCircle(){

    //this will sort any mesh into a circle

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


        //calculate the position in the circle
        posx = int(radius * cos(angle) + radius+100);
        posy = int(radius * sin(angle) + radius+100);

        //set the position
        n->setPosition(posx, posy);

        //increase the angle
        angle += degreeDistance;
    }


}


void Mesh::sortRow(){

    //first remove all waypoints of each connection, later the renderclass will add new waypoints if none are found
    foreach (Connection *c, this->getAllConnections()) {
        c->waypoints.clear();
    }

    //now sort all nodes in a circle
    int numberOfNodes = this->getAllNodes().size();

    //distance to the next node
    int nodeDistanceX = 250;
    int nodeDistanceY = 100;
    int numberOfRows = 4;
    int numberOfLines = numberOfNodes/numberOfRows + 1;

    int offsetX = 100;
    int offsetY = 100;

    int posx=0, posy=0;

    int counterMiddle = 0;
    int counterLeft = 0;
    int counterRight = 0;


    foreach (Node *n, this->getAllNodes()) {

        //put all nodes without input to the left side
        if (n->getInputGates()->size() == 0) {

            posx = 0;
            posy = nodeDistanceY * counterLeft;
            counterLeft++;

        }else if(n->getOutputGates()->size() == 0){

            posx = nodeDistanceX * numberOfRows;
            posy = nodeDistanceY * counterRight;
            counterRight++;

        }else{
            //nodes in the middle
            posx = nodeDistanceX+nodeDistanceX*(counterMiddle/numberOfLines);
            posy = nodeDistanceY*(counterMiddle%numberOfLines);
            counterMiddle++;
        }

        posx+=offsetX;
        posy+=offsetY;

        //set the position
        n->setPosition(posx, posy);

    }
}


void Mesh::sortForce(){

    //first we sort the mesh into a row
    this->sortRow();

    double deltaT = 10.00;

    double forceX;
    double forceY;
    double force;

    int difX;
    int difY;
    double distance;

    int posx;
    int posy;

    float push = 20;

    for (int ii = 0; ii < 200; ++ii) {
        foreach (Node *nodeI, this->getAllNodes()) {
            forceX = 0;
            forceY = 0;
            foreach (Node *nodeJ, this->getAllNodes()) {
                if (nodeI->getID() != nodeJ->getID()) {

                    //pull away from all nodes
                    difX = nodeI->x() - nodeJ->x();
                    difY = nodeI->y() - nodeJ->y();

                    distance = sqrt(pow(difX,2.0) + pow(difY,2.0));
                    if (distance > 1000) {
                        force = 0;
                    }else{
                        force = push/(distance+0.001);
                    }


                    double alpha = atan2(difY, difX)  ;




                    forceX += force *cos(alpha);
                    forceY += force *sin(alpha);


                }

            }


            posx = nodeI->x()+(forceX*deltaT);
            posy = nodeI->y()+(forceY*deltaT);


            nodeI->setPosition(posx, posy);

        }


        //pull towards connected Nodes
        foreach (Connection *C, this->getAllConnections()) {

            Node *nodeI = C->getDestNode();
            Node *nodeJ = C->getSrcNode();

            //calculate distance
            difX = nodeI->x() - nodeJ->x();
            difY = nodeI->y() - nodeJ->y();

            distance = sqrt(pow(difX,2.0) + pow(difY,2.0));

            if (distance <= 200 ) {
                force = 0;
            }else{
                force = distance*0.5; //a force is pulling them together
            }
            //qDebug()<< "";
            //qDebug() << "Ipos" << nodeI->x() << ";" << nodeI->y();
            //qDebug() << "Jpos" << nodeJ->x() << ";" << nodeJ->y();
            //qDebug() << "kraft " << force << " x " << difX << " y " << difY << " dis "<< distance;

            double alpha = atan2(difY, difX)  ;
            //qDebug() << "alpha: " << alpha;

            forceY = sin(alpha);
            //qDebug() << "sin " << forceY;

            forceY *= force;
            //qDebug() << "yforce: " << forceY;

            forceX = cos(alpha );
            //qDebug() << "cos " << forceX;

            forceX *= force;
            //qDebug()<< "xforce: " << forceX;

            //qDebug() << " forceX " << forceX << " forceY" << forceY;

            //pull NodeI closer
            posx = nodeI->x()-(forceX/2.0)+10;
            posy = nodeI->y()-(forceY/2.0);
            nodeI->setPosition(posx, posy);

            //pull NodeJ closer
            posx = nodeJ->x()+(forceX/2.0)-10;
            posy = nodeJ->y()+(forceY/2.0);
            nodeJ->setPosition(posx, posy);

            //qDebug() << "  Ipos after" << nodeI->x() << ";" << nodeI->y();
            //qDebug() << "  Jpos after" << nodeJ->x() << ";" << nodeJ->y();

        }
    }




    //remove all connection waypoints so they will be straight lines
    foreach (Connection *c, this->getAllConnections()) {
        c->waypoints.clear();
    }

    //move them into the view field
    float miny = 12345670.0;
    float minx = 12345670.0;
    foreach (Node *nodeI, this->getAllNodes()) {
        if (nodeI->x() < minx) {
            minx = nodeI->x();
        }

        if (nodeI->y() < miny) {
            miny = nodeI->y();
        }
    }
    foreach (Node *nodeI, this->getAllNodes()) {
        nodeI->setPosition(nodeI->x()-minx, nodeI->y()-miny);
    }



}

Mesh::~Mesh(){
    for(Connection *c : connectionsInMesh)
        delete c;
    for(Node *n : nodesInMesh)
        delete n;
}
