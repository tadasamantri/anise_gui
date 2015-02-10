#include <QMimeData>
#include <QDrag>

#include "mesheditorwidget.h"
#include "data.h"
#include "gatebutton.h"

MeshEditorWidget::MeshEditorWidget(QWidget *parent) : QWidget(parent) {
    connectFocusSignal();
    this->setMinimumHeight(5000);
    this->setMinimumWidth(5000);
    clearNewLine();
}

void MeshEditorWidget::connectFocusSignal(){
    connect(this, SIGNAL(onWidgetClicked(int)), Data::instance()->getMesh(),
            SLOT(setFocusMeshObject(int)));
}

void MeshEditorWidget::clearNewLine(){

    newLine = NewLine();
    newLine.drawLine = false;
    newLine.srcNodeID = -1;
    newLine.srcGateName = "";
    newLine.destNodeID = -1;
    newLine.destGateName = "";
    newLine.wayPoints = QVector<QPoint>();

}

void MeshEditorWidget::mousePressEvent(QMouseEvent *event) {

    DrawObject *child = 0;
    QLabel *labelChild = 0;
    GateButton *buttonChild = 0;


    // ensure a left-mouse-click
    if (!(event->button() == Qt::LeftButton)) {
        this->lineWayPoints.clear();
        return;
    }

    //---Button Check-----
    //Did I click on a Button (Gate)?
    buttonChild = dynamic_cast<GateButton *>(childAt(event->pos()));
    qDebug() << buttonChild;
    //If yes, buttonChild will not be 0
    if(buttonChild){
      qDebug() << "GatekLick via pressevent erkannt";
      return;
    }
    //---Label Check----------
    // check of clicking on label (picture)
    labelChild = dynamic_cast<QLabel *>(childAt(event->pos()));
    // if i clicked on a picture...
    if (labelChild)
        // ...then my parent will be drawobject hopefully
        child = dynamic_cast<DrawObject *>(labelChild->parent());

    //If it is still 0, i clicked on the meshfield itself
    if (!child) {

        if(newLine.drawLine){
            // add a way point for the line to draw
            this->newLine.wayPoints.push_back(event->pos());
        }

        else

            emit onWidgetClicked(-1);
        // return if no child at mouse position
        return;
    }

    // pressed on a child

    emit onWidgetClicked(child->ID);
    // set focus on it

    // relative point of mouse to child
    QPoint hotSpot = event->pos() - child->pos();
    QByteArray arrayData;
    QDataStream dataStream(&arrayData, QIODevice::WriteOnly);

    // stream data into the datastream. these data can be restored later in the drop event
    // data Stream
    dataStream << QPoint(hotSpot) << child->ID << event->pos();

    // qDebug() << "NODEID BEFORE DATASTREAM: " << child->nodeID;
    // something about mime data...
    // TODO correct mimedata
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/customthingy", arrayData);


    // hides the child so only the drag object at mouse position is shown
    child->hide();

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(child->getPicture());
    drag->setHotSpot(hotSpot);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) ==
            Qt::MoveAction)
        qDebug() << "never happens? IF it does check mesheditorwidget";

    else {
        // qDebug() << "drag end";
        child->show();
        // child->setPixmap(pixmap);
    }
}

void MeshEditorWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/customthingy")) {
        // qDebug() << "dragEnterEvent";
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}


void MeshEditorWidget::mouseMoveEvent(QMouseEvent *event){
    /*
    //will draw a line from newLine.drawLineStartPosition to mouse

    this->mousePosition = event->pos();

    this->newLine.drawLine = true;

    if (newLine.drawLine == true) {
        // TODO optimize the drawing
        // right now it redraws everything !
        this->repaint();
    }
    */
}

void MeshEditorWidget::dragMoveEvent(QDragMoveEvent *event) {
    // TODO track mouse for updating position
    // autoscroll
}

void MeshEditorWidget::dropEvent(QDropEvent *event) {
    // qDebug() << "dropEvent";

    // qDebug() << QString(event->mimeData()->data("application/customthingy"));
    QPoint DropPoint = event->pos();
    QPoint offset;
    int nodeID;

    QByteArray arrayData = event->mimeData()->data("application/customthingy");
    QDataStream dataStream(&arrayData, QIODevice::ReadOnly);








    //get the offset out of the mime, the offset is the distance from the mouse to the 0,0 position of the label/widget
    dataStream >> offset;

    //the id of the dragged object
    dataStream >> nodeID;







    DropPoint -= offset;

    qDebug() << "source of drop" << event->source()->objectName();
    qDebug() << "DropPoint" << DropPoint;

    if (event->source()->objectName() == "nodeCatalogContent") {

        // should create the correct node type and not a "testnode"
        QString _class;
        dataStream >> _class;

        Node *newNode = Data::instance()->nodeFactory->createNode(_class);

        newNode->setName("Node" + QString::number(Data::instance()->getMesh()->getCurrentID()));
        newNode->setPosition(DropPoint.x(), DropPoint.y());

        qDebug() << "new node of tpye " << _class << " created at position ("<< DropPoint.x() << "|" << DropPoint.y() << ")";


        Data::instance()->addNodeToMesh(newNode);
    }

    if (event->source()->objectName() == "meshField") {


        //get the start posisiotn of the drop from the mime
        // TODO die start position wird jetzt nur aus dem datastream gelesen wenn wir innerhalb vom meshfield draggen. aber man sollte dafür das application/datathingy benutzen
        QPoint dropstart ;
        dataStream >> dropstart;
        qDebug() << " dropstart " << dropstart.x() << " " << dropstart.y();

        // will move the correct node to the new position
        qDebug() << "event position" << event->pos();
        qDebug() << "mime:" << event->mimeData()->data("application/customthingy");
        qDebug() << "trying to move the node to the new position";
        Data::instance()->moveObjectInMesh(&dropstart, &DropPoint, nodeID);
    }
}

void MeshEditorWidget::paintEvent(QPaintEvent *event) {
    /*
    if (this->newLine.drawLine == true && !lineWayPoints.empty()) {
        // this will draw the vector with points as a line
        SingletonRender::instance()->newLine.drawLines(&lineWayPoints, &mousePosition);
    }

*/
    //draw all connections:

    SingletonRender::instance()->renderConnections();


}

bool MeshEditorWidget::containsID(int objectID) {
    foreach (QObject *child, this->children()) {
        DrawObject *castChild = static_cast<DrawObject *>(child);

        if (!castChild) continue;

        if (castChild->ID == objectID) return true;
    }
    return false;
}

void MeshEditorWidget::handleGateClick(int nodeID, QString gateName, QPoint position){


    qDebug() << "hey mein gateclick wurde erkannt!" << newLine.drawLine;


    //That means we are currently constructing a line
    if(newLine.drawLine){

        //Ask for Correctness of Connection

        newLine.destNodeID = nodeID;
        newLine.destGateName = gateName;
        newLine.wayPoints.push_back(position);

        //call Datastuff to create Connection
        // do this if connection is established
        Data::instance()->addConnectionToMesh(
                    NodeFactory::createConnection(newLine.srcNodeID, newLine.srcGateName, newLine.destNodeID, newLine.destGateName, newLine.wayPoints)
                    );

        this->clearNewLine();


    }


    //That means we are just starting a new Line
    else{

        //Ask for Correctness of Connection

        newLine.srcNodeID = nodeID;
        newLine.srcGateName = gateName;
        newLine.wayPoints.push_back(position);

        //Do Everything that Changed when Clicking on a gate
        newLine.drawLine = !(newLine.drawLine);



    }







}
