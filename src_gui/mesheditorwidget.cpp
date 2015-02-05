#include "mesheditorwidget.h"
#include "data.h"

MeshEditorWidget::MeshEditorWidget(QWidget *parent) : QWidget(parent) {
    connect(this, SIGNAL(onWidgetClicked(int)), Data::instance()->getMesh(),
            SLOT(setFocusMeshObject(int)));
    this->setMinimumHeight(5000);
    this->setMinimumWidth(5000);
    drawLine = false;
}

void MeshEditorWidget::mousePressEvent(QMouseEvent *event) {
    // ensure a left-mouse-click
    if (!(event->button() == Qt::LeftButton)) {
        this->lineWayPoints.clear();
        return;
    }

    qDebug() << childAt(event->pos());
    // get child at mouse position
    // TODO NOTLÖSUNG - HAS TO BE CHANGED
    // PROPERLY------------------------------------------

    DrawObject *child;

    // CHECK OF CLICKING ON LABEL (PICTURE)
    QLabel *grandChild = dynamic_cast<QLabel *>(childAt(event->pos()));

    // IF I CLICKED ON PICTURE...
    if (grandChild) {
        // THEN MY PARENT WILL BE THE DRAWOBJECT (HOPEFULLY)
        child = dynamic_cast<DrawObject *>(grandChild->parent());
    } else
        // IF NOT THEN MAYBE I CLICKED ON THE WIDGET (CAUSE THE PICTURES DONT FILL
        // UP ALL THE SPACE OF THE WIDGET)
        child = dynamic_cast<DrawObject *>(childAt(event->pos()));
    //---------------------------------------------------------------------------------------

    // IF IT IS STILL 0, THEN I CLICKED ON MESHFIELD NOT A NODE
    if (!child) {
        // add a way point for the line to draw
        this->lineWayPoints.push_back(event->pos());

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
    // TODO correct use of node ID
    dataStream << QPoint(hotSpot) << child->ID;

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

void MeshEditorWidget::mouseMoveEvent(QMouseEvent *event) {
    // will draw a line from drawLineStartPosition to mouse
    this->mousePosition = event->pos();

    this->drawLine = true;

    if (drawLine == true) {
        // TODO optimize the drawing
        // right now it redraws everything !
        this->repaint();
    }
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
    dataStream >> offset;
    dataStream >> nodeID;

    DropPoint -= offset;

    qDebug() << "source of drop" << event->source()->objectName();
    qDebug() << "DropPoint" << DropPoint;

    if (event->source()->objectName() == "nodeCatalogContent") {
        // should create the correct node type and not a "testnode"
        QString _class;
        dataStream >> _class;
        Node *newNode = Data::instance()->nodeFactory->createNode(_class);
        newNode->setName(
                    "Node" + QString::number(Data::instance()->getMesh()->getCurrentID()));
        newNode->setPosition(DropPoint.x(), DropPoint.y());
        qDebug() << "new node of tpye " << _class << " created at position ("
                 << DropPoint.x() << "|" << DropPoint.y() << ")";
        Data::instance()->addNodeToMesh(newNode);
    }

    if (event->source()->objectName() == "meshField") {
        // will move the correct node to the new position
        qDebug() << "event position" << event->pos();
        qDebug() << "mime:" << event->mimeData()->data("application/customthingy");
        qDebug() << "trying to move the node to the new position";
        Data::instance()->moveNodeInMesh(&DropPoint, nodeID);
    }
}

void MeshEditorWidget::paintEvent(QPaintEvent *event) {
    if (this->drawLine == true && !lineWayPoints.empty()) {
        // this will draw the vector with points as a line
        SingletonRender::instance()->drawLines(&lineWayPoints, &mousePosition);
    }
}

bool MeshEditorWidget::containsID(int objectID) {
    foreach (QObject *child, this->children()) {
        DrawObject *castChild = static_cast<DrawObject *>(child);

        if (!castChild) continue;

        if (castChild->ID == objectID) return true;
    }
    return false;
}
