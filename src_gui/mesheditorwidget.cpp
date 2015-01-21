#include "mesheditorwidget.h"
#include "nodefactory.h"
#include "data.h"
#include "node.h"
#include "drawobject.h"

MeshEditorWidget::MeshEditorWidget(QWidget *parent) : QWidget(parent) {}

void MeshEditorWidget::mousePressEvent(QMouseEvent *event) {
  // ensure a left-mouse-click
  if (!(event->button() == Qt::LeftButton)) {
    return;
  }

  // get child at mouse position
  DrawObject *child = static_cast<DrawObject *>(childAt(event->pos()));
  if (!child) {
    // return if no child at mouse position
    return;
  }

  // pressed on a child

  // relative point of mouse to child
  QPoint hotSpot = event->pos() - child->pos();

  QByteArray Data;
  QDataStream dataStream(&Data, QIODevice::WriteOnly);
  // TODO correct use of node ID
  dataStream << QPoint(hotSpot) << child->numberOfNode;

  // something about mime data...
  // TODO correct mimedata
  QMimeData *mimeData = new QMimeData;
  mimeData->setData("application/customthingy", Data);

  // hides the child so only the drag object at mouse position is shown
  child->hide();

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->setPixmap(*child->pixmap());
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

  Node *newNode = NodeFactory::createTestNode();

  // qDebug() << QString(event->mimeData()->data("application/customthingy"));
  QPoint DropPoint = event->pos();
  QPoint offset;
  int numberOfNode;

  QByteArray Data = event->mimeData()->data("application/customthingy");
  QDataStream dataStream(&Data, QIODevice::ReadOnly);
  dataStream >> offset;
  dataStream >> numberOfNode;

  DropPoint -= offset;

  // qDebug() << "DropPoint" << DropPoint;

  newNode->setPosition(DropPoint.x(), DropPoint.y());

  // qDebug() << "x:" << newNode->position_x << " y: " << newNode->position_y;
  // qDebug() << "source of drop"<<event->source()->objectName();
  if (event->source()->objectName() == "nodeCatalogContent")
    Data::instance()->addNodeToMesh(newNode);
  if (event->source()->objectName() == "meshField")
    Data::instance()->moveNodeInMesh(&DropPoint, numberOfNode);
}
