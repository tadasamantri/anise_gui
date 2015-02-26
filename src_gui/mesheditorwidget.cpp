#include <QMimeData>
#include <QDrag>
#include <QToolTip>
#include "mesheditorwidget.h"
#include "data.h"
#include "gatebutton.h"

MeshEditorWidget::MeshEditorWidget(QWidget *parent) : QWidget(parent) {
    connectSignals();
    //connectDrawModeSingal();
    this->setMinimumHeight(5000);
    this->setMinimumWidth(5000);
    clearNewLine();
}

void MeshEditorWidget::connectSignals() {
    connect(this, SIGNAL(onWidgetClicked(int)), Data::instance()->getMesh(),
            SLOT(setFocusMeshObject(int)));
    connect(this,SIGNAL(drawLineModeChanged()), this, SLOT(changeLineDrawMode()));
}

void MeshEditorWidget::clearNewLine() {
    newLine = NewLine();
    newLine.drawLine = false;
    newLine.srcNodeID = -1;
    newLine.srcGateName = "";
    newLine.destNodeID = -1;
    newLine.destGateName = "";
    newLine.wayPoints = QVector<QPoint>();
    emit drawLineModeChanged();
}

void MeshEditorWidget::mousePressEvent(QMouseEvent *event) {
    // ensure a left-mouse-click
    if (!(event->button() == Qt::LeftButton)) {
        this->clearNewLine();
        return;
    }

    // drawLine Mode
    if (newLine.drawLine) {
        // Check if i clicked on a child
        QObject *child = childAt(event->pos());

        if(!child){
            // add a way point for the line to draw
            this->newLine.wayPoints.push_back(event->pos());
        }
    }

    // Normal Mode (currently not drawing a Connection)

    else {
        DrawObject *child = 0;
        QLabel *labelChild = 0;
        //GateButton *buttonChild = 0;

        //---Clicked on Node?----------
        // check of clicking on label (picture)
        labelChild = dynamic_cast<QLabel *>(childAt(event->pos()));
        // if i clicked on a picture...
        if (labelChild)
            // ...then my parent will be drawobject hopefully
            child = dynamic_cast<DrawObject *>(labelChild->parent());

        // If it is still 0, i clicked on the meshfield itself
        if (!child) {
            emit onWidgetClicked(-1);
            // return if no child at mouse position
            return;
        }

        // pressed on a child
        // set focus on it
        emit onWidgetClicked(child->ID);



        // relative point of mouse to child
        QPoint hotSpot = event->pos() - child->pos();
        QByteArray arrayData;
        QDataStream dataStream(&arrayData, QIODevice::WriteOnly);

        // stream data into the datastream. these data can be restored later in the
        // drop event
        // data Stream
        dataStream << QPoint(hotSpot) << child->ID << event->pos();

        // something about mime data...
        // TODO correct mimedata
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("ANISE-GUI/drawobject", arrayData);

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
            child->show();
        }
    }
}

void MeshEditorWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("ANISE-GUI/drawobject")) {
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

void MeshEditorWidget::mouseMoveEvent(QMouseEvent *event) {

    //update mouse position
    this->mousePosition = event->pos();
    this->repaint();

}

void MeshEditorWidget::dragMoveEvent(QDragMoveEvent *event) {
    //update mouse position
    this->mousePosition = event->pos();
    this->repaint();

}

void MeshEditorWidget::dropEvent(QDropEvent *event) {

    QPoint DropPoint = event->pos();
    QPoint offset;
    int nodeID;

    QByteArray arrayData = event->mimeData()->data("ANISE-GUI/drawobject");
    QDataStream dataStream(&arrayData, QIODevice::ReadOnly);

    // get the offset out of the mime, the offset is the distance from the mouse
    // to the 0,0 position of the label/widget
    dataStream >> offset;

    // the id of the dragged object
    dataStream >> nodeID;

    DropPoint -= offset;

    if (event->source()->objectName() == "nodeCatalogContent") {
        // should create the correct node type and not a "testnode"
        QString _class;
        dataStream >> _class;

        Node *newNode = Data::instance()->nodeFactory->createNode(_class);

        newNode->setName(_class);
        newNode->setPosition(DropPoint.x(), DropPoint.y());
        clearNewLine();
        Data::instance()->addNodeToMesh(newNode);
    }

    else if (event->source()->objectName() == "meshField") {
        // get the start posisiotn of the drop from the mime
        // TODO die start position wird jetzt nur aus dem datastream gelesen wenn
        // wir innerhalb vom meshfield draggen. aber man sollte dafür das
        // application/datathingy benutzen
        QPoint dropstart;
        dataStream >> dropstart;

        // will move the correct node to the new position
        Data::instance()->moveObjectInMesh(&dropstart, &DropPoint, nodeID);
    }
}

void MeshEditorWidget::paintEvent(QPaintEvent *event) {



    //qDebug() << "maus posi: " << mousePosition;

    if ( newLine.drawLine == true) {
        //qDebug() << "trying to draw a line to mouse";
        // this will draw the vector with points as a line
        SingletonRender::instance()->drawLines(&newLine.wayPoints,&mousePosition);
    }


    // draw all connections:

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

bool MeshEditorWidget::handleGateClick(int nodeID, QString gateName,
                                       QPoint position) {

    //Do nothing if you clicked on wrong Gate
    if(!correctGate(nodeID, gateName))
        return false;


    // That means we are currently constructing a line
    if (newLine.drawLine) {
        // Ask for Correctness of Connection
    
        newLine.destNodeID = nodeID;
        newLine.destGateName = gateName;
        //this is the InputGate where Connection ends
        newLine.wayPoints.push_back(position + SingletonRender::instance()->getInputGateDrawOffset());
        // call Datastuff to create Connection
        // do this if connection is established
        /*int ID = */Data::instance()->addConnectionToMesh(NodeFactory::createConnection(
                                                               newLine.srcNodeID, newLine.srcGateName, newLine.destNodeID,
                                                               newLine.destGateName, newLine.wayPoints));
        this->clearNewLine();

    }

    // That means we are just starting a new Line
    else {
        // Ask for Correctness of Connection

        newLine.srcNodeID = nodeID;
        newLine.srcGateName = gateName;
        //this is the OutputGatePosition where Connection starts
        newLine.wayPoints.push_back(position + SingletonRender::instance()->getOutputGateDrawOffset());

        // Do Everything that Changed when Clicking on a gate
        newLine.drawLine = !(newLine.drawLine);
        emit drawLineModeChanged();
    }
    return true;
}



void MeshEditorWidget::changeLineDrawMode()
{
    if(newLine.drawLine){
        setCursor(Qt::CrossCursor);
        QToolTip::showText(cursor().pos(), QString("exit line draw mode with right click"), this, Data::instance()->getMainWindow()->ui->mesh_edt_area->rect(),4000);
    }
    else setCursor(Qt::ArrowCursor);
}

bool MeshEditorWidget::correctGate(int nodeID, QString gateName){
    
    Node *node = Data::instance()->getMesh()->getNodeByID(nodeID);
    Gate *endGate;


    if(!node)
        return false;

    endGate = node->getGateByName(gateName);

    if(!endGate)
        return false;

    if(newLine.drawLine){

        Gate *srcGate = Data::instance()->getMesh()->getNodeByID(newLine.srcNodeID)->getGateByName(newLine.srcGateName);

        return endGate->getDirection() && srcGate->getType() == endGate->getType() && newLine.srcNodeID != nodeID;
        
    }

    else{
        return !endGate->getDirection();
    }
}
