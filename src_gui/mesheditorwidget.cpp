#include "mesheditorwidget.h"
#include "nodefactory.h"
#include "dataholder.h"
#include "data.h"
#include "node.h"
#include "drawobject.h"

MeshEditorWidget::MeshEditorWidget(QWidget *parent) : QWidget(parent) {}

void MeshEditorWidget::mousePressEvent(QMouseEvent *event) {
  qDebug() << "click";
  DrawObject *child = static_cast<DrawObject *>(childAt(event->pos()));
  if (!child) {
    qDebug() << "no child";
    return;
  }

  // ensure a left-mouse-click

  if (!(event->button() == Qt::LeftButton)) {
    qDebug() << "that was not the left mouse button!\n\n";
    return;
  }

  qDebug() << "child";
  QPoint hotSpot = event->pos() - child->pos();

  QByteArray Data;
  QDataStream dataStream(&Data, QIODevice::WriteOnly);
  dataStream /*<< child */ << QPoint(hotSpot) << child->numberOfNode;

  // something about mime data...
  QMimeData *mimeData = new QMimeData;
  mimeData->setData("application/customthingy", Data);

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->setPixmap(*child->pixmap());
  drag->setHotSpot(hotSpot);
  /*
   // This will change the color of the drag icon; could also be used to
   generate new images
      QPixmap tempPixmap = child->pixmap();
      QPainter painter;
      painter.begin(&tempPixmap);
      painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
      painter.end();
  */
  // child->setPixmap(tempPixmap);

  if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) ==
      Qt::MoveAction)
    qDebug() << "never happens?";
  // child->close();
  else {
    qDebug() << "drag end";
    child->show();
    // child->setPixmap(pixmap);
  }
}

// ENDLÖSUNG FÜR SPÄTER
/*void MeshEditorWidget::mousePressEvent(QMouseEvent *event)
 {

     QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
     if(!child){
         qDebug() << "no child";
         return;
     }

     if (event->button() == Qt::LeftButton)
         this->dragStartPosition = event->pos();
     qDebug() << "Im a Child";
     //child->hide();
 }
*/

void MeshEditorWidget::dragEnterEvent(QDragEnterEvent *event) {
  qDebug() << "dragEnterEvent";
  if (event->mimeData()->hasFormat("application/customthingy")) {
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
  // qDebug()<<"dragMoveEvent";
}

void MeshEditorWidget::dropEvent(QDropEvent *event) {
  qDebug() << "dropEvent";

  Node *newNode = NodeFactory::createTestNode();

  // qDebug() << QString(event->mimeData()->data("application/customthingy"));
  QPoint DropPoint = event->pos();
  QPoint offset;
  int numberOfNode;

  QByteArray Data = event->mimeData()->data("application/customthingy");
  QDataStream dataStream(&Data, QIODevice::ReadOnly);
  dataStream >> offset;
  dataStream >> numberOfNode;
  qDebug() << "Number of Node" << numberOfNode;

  DropPoint -= offset;

  // qDebug()<< "event->pos()" << event->pos();
  qDebug() << "DropPoint" << DropPoint;

  newNode->setPosition(DropPoint.x(), DropPoint.y());

  qDebug() << "x:" << newNode->position_x << " y: " << newNode->position_y;
  qDebug() << event->source()->objectName();
  if (event->source()->objectName() == "nodeCatalogContent")
    DataHolder::getData()->addNodeToMesh(newNode);
  if (event->source()->objectName() == "meshField")
    DataHolder::getData()->moveNodeInMesh(&DropPoint, numberOfNode);
}
// ENDLÖSUNG FÜR SPÄTER
/*
void MeshEditorWidget::mouseMoveEvent(QMouseEvent *event)
 {


     if (!(event->buttons() & Qt::LeftButton))
         return;
     if ((event->pos() - dragStartPosition).manhattanLength() <
QApplication::startDragDistance())
         return;

     QPoint hotSpot = QPoint(0,0);

     QByteArray Data ;
     QDataStream dataStream(&Data, QIODevice::WriteOnly);
     dataStream << QPoint(hotSpot);


     QDrag *drag = new QDrag(this);
     QMimeData *mimeData = new QMimeData;

     mimeData->setData("application/customthingy", Data);
     drag->setMimeData(mimeData);

     Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

 }
*/
