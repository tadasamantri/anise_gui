#include "mesheditorwidget.h"
#include "data.h"

MeshEditorWidget::MeshEditorWidget(QWidget *parent) : QWidget(parent) {

    connect(this, SIGNAL(onWidgetClicked(int)), Data::instance()->getMesh(), SLOT(setFocusMeshObject(int)));


}

void MeshEditorWidget::mousePressEvent(QMouseEvent *event) {
    // ensure a left-mouse-click
    if (!(event->button() == Qt::LeftButton)) {
        return;
    }

    // get child at mouse position
    DrawObject *child = static_cast<DrawObject *>(childAt(event->pos()));
    if (!child) {
        emit onWidgetClicked(-1);
        // return if no child at mouse position
        return;
    }

    // pressed on a child

    emit onWidgetClicked(child->nodeID);
    // set focus on it



    // relative point of mouse to child
    QPoint hotSpot = event->pos() - child->pos();

    QByteArray arrayData;
    QDataStream dataStream(&arrayData, QIODevice::WriteOnly);
    // TODO correct use of node ID
    dataStream << QPoint(hotSpot) << child->nodeID;

    // something about mime data...
    // TODO correct mimedata
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/customthingy", arrayData);

    // hides the child so only the drag object at mouse position is shown
    child->hide();

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    //drag->setPixmap(*child->pixmap());
    drag->setHotSpot(hotSpot);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) ==
            Qt::MoveAction)
        qDebug() << "never happens? IF it does check mesheditorwidget";
    // child->close();
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

void MeshEditorWidget::dragMoveEvent(QDragMoveEvent *event) {
    // TODO track mouse for updating position
    // qDebug()<<"dragMoveEvent" << event->pos();
}

void MeshEditorWidget::dropEvent(QDropEvent *event) {
    // qDebug() << "dropEvent";


    // qDebug() << QString(event->mimeData()->data("application/customthingy"));
    QPoint DropPoint = event->pos();
    QPoint offset;
    int nodeID;

    QByteArray arrayData = event->mimeData()->data("application/customthingy");
    QDataStream dataStream(&arrayData, QIODevice::ReadOnly);
    dataStream >> offset;
    dataStream >> nodeID;

   // DropPoint -= offset;





    qDebug() << "source of drop"<<event->source()->objectName();
    qDebug() << "DropPoint" << DropPoint;

    if (event->source()->objectName() == "nodeCatalogContent"){

        //should create the correct node type and not a "testnode"
        Node *newNode = Data::instance()->nodeFactory->createTestNode();
        newNode->setPosition(DropPoint.x(), DropPoint.y());
        Data::instance()->addNodeToMesh(newNode);}

    if (event->source()->objectName() == "meshField"){
        //will move the correct node to the new position
        qDebug()<<"event position"<< event->pos();
        qDebug() <<"mime:" <<event->mimeData()->data("application/customthingy");
        qDebug() << "trying to move the node to the new position";
        Data::instance()->moveNodeInMesh(&DropPoint, nodeID);
    }
}

void MeshEditorWidget::paintEvent(QPaintEvent *event){
    //TTtest linie, please ignore
    SingletonRender::instance()->drawLine(10, 20, 80, 60);
}

bool MeshEditorWidget::containsID(int objectID){


    foreach(QObject *child, this->children()){

        DrawObject *castChild = static_cast<DrawObject *>(child);

        if(!castChild)
            continue;

        if(castChild->nodeID == objectID)
            return true;

    }
    return false;

}



