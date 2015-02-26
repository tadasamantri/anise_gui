#include <QDir>
#include <limits>
#include "singletonrender.h"
#include "testdrawobject.h"
#include "nodetypelabel.h"
#include "ui_mainwindow.h"
#include "data.h"

// Global static pointer used to ensure a single instance of the class.
SingletonRender *SingletonRender::m_pInstance = NULL;

SingletonRender::SingletonRender() {
    // initialize all maps
    this->allDrawnNodes = QMap<int, DrawObject *>();
    this->allConnections = QMap<int, QVector<DrawObject *> >();
    this->allImages = QMap<QString, QPixmap *>();

    // load all images
    if (loadImages() == true) {
        qDebug() << "images loaded successfully";
    } else {
        qDebug() << "images loading failed";
    }
}

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/
SingletonRender *SingletonRender::instance() {
    if (!m_pInstance)  // Only allow one instance of class to be generated.
        m_pInstance = new SingletonRender;

    return m_pInstance;
}

void SingletonRender::renderConnections() {
    foreach (QVector<QLine> lineVec, this->allLines) {
        foreach (QLine line, lineVec) { this->drawLine(line); }
    }
}

// will render a given connection
void SingletonRender::renderConnection(Connection *conToRender, int ID) {
    // will draw a line connecting all connection joints of a connection

    if (!allConnections.contains(ID)) {
        QVector<QLine> tempVec;
        // create new Lines
        for (int index = 0; index < conToRender->waypoints.size() - 1; ++index) {
            tempVec << QLine(conToRender->waypoints.at(index),
                             conToRender->waypoints.at(index + 1));
        }
        this->allLines.insert(ID, tempVec);

        // create a new Vector for all Joints
        QVector<DrawObject *> jointVector;

        // now draw all joints
        foreach (QPoint joint, conToRender->waypoints) {
            if (!allImages.contains("joint.png")) {
                qDebug() << "joint.png missing! cant draw connections!";
                return;
            }

            // create a new Drawobject and save some space for the image
            DrawObject *ConnectionJointDrawObject = new DrawObject(
                        ID, QPoint(0, 0), allImages.value("joint.png")->width(),
                        allImages.value("joint.png")->height(), this->ui->meshField);

            // Add the picture to the draw object
            ConnectionJointDrawObject->addPicture(allImages["joint.png"],
                    QPoint(0, 0));

            // add a tooltip
            ConnectionJointDrawObject->setToolTip("click To Drag");

            // add the DrawObject into the Vector
            jointVector.push_back(ConnectionJointDrawObject);
        }
        // Add the vector into the map
        allConnections.insert(ID, jointVector);

    } else {
        // update the lines
        QVector<QLine> tempVec;
        // this->allLines.clear();
        // create new Lines
        for (int index = 0; index < conToRender->waypoints.size() - 1; ++index) {
            tempVec << QLine(conToRender->waypoints.at(index),
                             conToRender->waypoints.at(index + 1));
        }
        this->allLines.insert(ID, tempVec);
    }



    //move all joints to the correct position
    for (int index = 1; index < allConnections[ID].size()-1; ++index) {


        DrawObject* joint = allConnections[ID].at(index);

        // calculate the middle of the Image
        int posxOffset = -joint->width() / 2;
        int posyOffset = -joint->height() / 2;

        joint->move(conToRender->waypoints.at(index).x()+posxOffset, conToRender->waypoints.at(index).y()+posyOffset);
        joint->show();
    }
}

// will have to be called from a paint event!
void SingletonRender::drawLine(double start_x, double start_y, double end_x,
                               double end_y) {
    QPainter painter(this->ui->meshField);

    QLineF line(start_x, start_y, end_x, end_y);
    painter.setPen(lineColor);
    painter.drawLine(line);
    // painter.draw
}

// will have to be called from a paint event!
void SingletonRender::drawLine(QLine line) {
    QPainter painter(this->ui->meshField);

    // QLineF line(start_x, start_y, end_x, end_y);
    painter.setPen(lineColor);
    painter.drawLine(line);
    // painter.draw
}

// will have to be called from a paint event!
void SingletonRender::drawLine(QPoint start, QPoint end) {
    QPainter painter(this->ui->meshField);

    QLineF line(start, end);
    painter.setPen(lineColor);
    painter.drawLine(line);
    // painter.draw
}

// will have to be called from a paint event!
void SingletonRender::drawLines(QVector<QPoint> *pointVector) {
    if (pointVector->empty()) {
        qDebug() << "tried to Draw an empty point Vector in SingleTon render!";
        return;
    }

    // draws a line from point to point
    QPainter painter(this->ui->meshField);
    painter.setPen(Qt::red);

    QVector<QPoint> copy(*pointVector);

    copy.remove(0);

    painter.drawLines(*pointVector);
    painter.drawLines(copy);
}

// will have to be called from a paint event!
void SingletonRender::drawLines(QVector<QPoint> *pointVector, QPoint *point) {
    if (pointVector->empty()) {
        qDebug() << "tried to Draw an empty point Vector in SingleTon render!";
        return;
    }
    this->drawLines(pointVector);
    this->drawLine(pointVector->last(), *point);
}

// loads all images in the ../DataIimages folder.
// saves them in the map "allImages"
bool SingletonRender::loadImages() {
    bool result = true;
    QStringList listOfFiles;

    // go into the right Directory using a Qdir object
    QDir directory;
    directory.cd("Data/Images/");

    // fill the list of Files with all filenames insode this directory
    if (directory.exists() == true) {
        // qDebug() << "directory.exists() == true";
        listOfFiles = directory.entryList();
    } else {
        qDebug() << "directory.exists() == false";
        result = false;
    }

    // remove all filenames who are not a png file
    listOfFiles = listOfFiles.filter(".png");

    // loads all images;
    // if one coudlnt be loaded it will return false
    for (int i = 0; i < listOfFiles.size(); ++i) {
        if (result == true) {
            QPixmap *temp = new QPixmap();

            // set the result true if loeaded succesfully, false if not
            result =
                    temp->load(directory.absolutePath().append("/" + listOfFiles.at(i)));

            // if failed print a debug message
            if (result == false) {
                qDebug() << "loaded image: "
                         << directory.absolutePath().append(listOfFiles.at(i)) << " "
                         << result;
            }

            // set transparency to magic pink
            temp->setMask(temp->createMaskFromColor(Qt::magenta));

            allImages.insert(listOfFiles.at(i), temp);
        }
    }

    //setDrawOffsets For Gates
    if(allImages.contains("gate.png")){

        QPixmap *pic = allImages.value("gate.png");
        int width = pic->width();
        int height = pic->height();

        this->setInputGateDrawOffset(QPoint(0, height/2));
        this->setOutputGateDrawOffset(QPoint(width, height/2));

    }
    return result;
}

void SingletonRender::setUi(Ui::MainWindow *ui) { this->ui = ui; }

void SingletonRender::renderNode(Node *nodeToRender, int nodeID) {
    if (!allDrawnNodes.contains(nodeID)) {
        // some Variables needed often
        int gateHeight = allImages["gate.png"]->height();
        int gateOffset = 10;
        QString typeName = nodeToRender->getType();

        // find out how high the node is depending on the number of gates
        int maxNumberGates = nodeToRender->getInputGates()->size();
        if (maxNumberGates < nodeToRender->getOutputGates()->size())
            maxNumberGates = nodeToRender->getOutputGates()->size();

        // Set height of DrawObject
        int drawObjectHeight = maxNumberGates * (gateHeight + gateOffset);
        if (drawObjectHeight < 50) drawObjectHeight = 50;

        // create a Drawobject
        DrawObject *NodeDrawObject = new DrawObject(
                    nodeID,
                    QPoint(int(nodeToRender->position_x), int(nodeToRender->position_y)),
                    100, drawObjectHeight, this->ui->meshField);

        if (allImages.contains("body.png")) {
            // Draw the body
            NodeDrawObject->addPicture(allImages["body.png"], QPoint(15, 0),
                    typeName);

        } else {
            qDebug() << "body.png did not load correctly!";
        }

        if (allImages.contains("gate.png")) {
            // Draw the gates
            QVector<Gate *> *inputGates = nodeToRender->getInputGates();
            QVector<Gate *> *outputGates = nodeToRender->getOutputGates();
            int numberInputGates = inputGates->size();
            int numberOutputGates = outputGates->size();

            for (int i = 0; i < numberInputGates; i++) {
                NodeDrawObject->addGateButton(
                            allImages["gate.png"], QPoint(0, i * (gateHeight + gateOffset) + 5),
                        inputGates->at(i)->getName(), inputGates->at(i)->getType(), true);
            }

            for (int i = 0; i < numberOutputGates; i++) {
                NodeDrawObject->addGateButton(
                            allImages["gate.png"],
                        QPoint(75, i * (gateHeight + gateOffset) + 5),
                        outputGates->at(i)->getName(), outputGates->at(i)->getType(),false);
            }

        } else {
            qDebug() << "body.png did not load correctly!";
        }

        NodeDrawObject->setToolTip(nodeToRender->getDescription());
        allDrawnNodes.insert(nodeID, NodeDrawObject);
    }
    // TODO should use layouts instead of hardcoded position!
    allDrawnNodes.value(nodeID)
            ->move(nodeToRender->position_x, nodeToRender->position_y);

    allDrawnNodes.value(nodeID)->show();
}

void SingletonRender::renderMesh(Mesh *workMesh) {
    QMap<int, Node *> temp = workMesh->nodesInMesh;

    // calls render method for every node in the mesh
    foreach (int ID, temp.keys()) {
        // Only do it if a Node with this ID exists in the mesh
        if (temp.contains(ID)) {
            renderNode(temp.value(ID), ID);
        } else {
            qDebug() << "tried to render a node that doesnt exist!";
        }
    }

    // calls render method for each connection in the mesh
    foreach (int ID, workMesh->connectionsInMesh.keys()) {
        renderConnection(workMesh->connectionsInMesh.value(ID), ID);
    }

    //renderConnections(); //needs to be called in the paint event of mesheditor widget
    this->ui->mesh_edt_area->repaint();
}

void SingletonRender::clearMeshField() { clearAll(ui->meshField); }

void SingletonRender::renderNodeType(Node *nodeToRender, QWidget *parent,
                                     int position) {
    // TODO code dublication in renderNode and renderNodeType!
    NodeTypeLabel *NodeDrawObject = new NodeTypeLabel(parent);
    renderNode(nodeToRender,std::numeric_limits<int>::max() -1);
    DrawObject *o = allDrawnNodes.value(std::numeric_limits<int>::max() -1);
    // Zeichne den hintergrund:
    NodeDrawObject->setPixmap(o->getPicture());
    NodeDrawObject->setMask(o->mainMaskUnhighlighted);
    deleteMeshDrawing(std::numeric_limits<int>::max() -1);
    NodeDrawObject->setGeometry(0, 0, 50, 50);
    NodeDrawObject->adjustSize();
    // Generate a Tooltip
    QString type = nodeToRender->getType();
    NodeDrawObject->setType(type);
    QString toolTip;
    toolTip = "NodeClass = " + nodeToRender->getType() + "\nInputs: " +
            QString::number(nodeToRender->getInputGates()->size()) +
            "\nOutputs: " +
            QString::number(nodeToRender->getOutputGates()->size());
    if (nodeToRender->getDescription() != "")
        toolTip += "\n" + nodeToRender->getDescription();
    NodeDrawObject->setToolTip(toolTip);
    // TODO should use layouts instead of hardcoded position!
    NodeDrawObject->move(5 + position * NodeDrawObject->width(), 5);


    QLabel *typeLabel = new QLabel();
    typeLabel->setText(type);
    typeLabel->setGeometry(5 + position * NodeDrawObject->width(), NodeDrawObject->height() + 10, NodeDrawObject->width(), 20);
    typeLabel->show();


    NodeDrawObject->show();
}

void SingletonRender::renderCatalogContent(QVector<Node> NodeVektor) {
    QWidget *CatalogParent = ui->nodeCatalogContent;
    int position = 0;
    // TODO scroll weite sollte nicht hard coded sein
    //CatalogParent->setMinimumHeight(NodeVektor.size() * 60 + 10);
    foreach (Node nodeTyp, NodeVektor) {
        renderNodeType(&nodeTyp, CatalogParent, position);
        position++;
    }
}

void SingletonRender::clearAll(QWidget *parent) {
    //
    while (QWidget *w = parent->findChild<DrawObject *>()) {
        w->deleteLater();
        delete w;
    }

    // TODO NOT COOL THIS SOLUTION!
    if (parent == ui->meshField) {
        this->allDrawnNodes = QMap<int, DrawObject *>();
        this->allConnections = QMap<int, QVector<DrawObject *> >();
        this->allLines = QMap<int, QVector<QLine>>();
    }
}
QPoint SingletonRender::getInputGateDrawOffset() const
{
    return inputGateDrawOffset;
}

void SingletonRender::setInputGateDrawOffset(const QPoint &value)
{
    inputGateDrawOffset = value;
}

QPoint SingletonRender::getOutputGateDrawOffset() const
{
    return outPutGateDrawOffset;
}

void SingletonRender::setOutputGateDrawOffset(const QPoint &value)
{
    outPutGateDrawOffset = value;
}


bool SingletonRender::deleteMeshDrawing(int objectID) {
    bool success = false;
    if(allDrawnNodes.contains(objectID)){
        DrawObject *childToDelete = allDrawnNodes[objectID];
        allDrawnNodes.remove(objectID);
        childToDelete->deleteLater();
        success =  !allDrawnNodes.contains(objectID);
    }
    else if(allConnections.contains(objectID) && allLines.contains(objectID)){
        QVector<DrawObject *> childrenToDelete = allConnections[objectID];
        allLines.remove(objectID);
        allConnections.remove(objectID);
        for(DrawObject *o : childrenToDelete)
            o->deleteLater();
        success = !(allLines.contains(objectID) || allConnections.contains(objectID));
    }
    if(success)
        ui->meshField->repaint();
    return success;
}

QVector<int> *SingletonRender::getChildrenIDs() {
    QVector<int> *ids = new QVector<int>();
    QObjectList children = this->ui->meshField->children();

    foreach (QObject *child, children) {
        DrawObject *node = dynamic_cast<DrawObject *>(child);

        if (node != NULL) ids->push_back(node->ID);
    }

    return ids;
}

void SingletonRender::dehighlightObject(int ID){

    if(allDrawnNodes.contains(ID)){

        allDrawnNodes.value(ID)->dehighlight();
    }

    if(allConnections.contains(ID)){



        foreach(DrawObject *joint , this->allConnections.value(ID)){

            joint->dehighlight();
        }

    }
}

void SingletonRender::highlightObject(int ID){

    if(allDrawnNodes.contains(ID))

        allDrawnNodes.value(ID)->highlight();

    if(allConnections.contains(ID)){

        foreach(DrawObject *joint , this->allConnections.value(ID)){

            joint->highlight();
        }
    }
}


void SingletonRender::highlightGates(QString gateType){

    foreach(DrawObject *node, allDrawnNodes){

        node->highlightGates(gateType);
    }


}

void SingletonRender::dehighlightGates(){

    foreach(DrawObject *node, allDrawnNodes){

        node->dehighlightGates();
    }

}


QPixmap *SingletonRender::getImage(QString name){

    if(allImages.contains(name))
        return allImages.value(name);
    return new QPixmap();

}



