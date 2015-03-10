#include "dragwidget.h"

#include <QDrag>
#include <QMimeData>
#include "drawobject.h"
#include "nodefactory.h"
#include "nodetypelabel.h"

DragWidget::DragWidget(QWidget *parent) : QWidget(parent) {
    setWindowOpacity(100);
}

void DragWidget::mousePressEvent(QMouseEvent *event) {
    /**
      * User pressed the mouse button
      * Ensure a left mouse click
      */
    if (!(event->button() == Qt::LeftButton)) {
        qDebug() << "that was not the left mouse button!\n";
        return;
    }

    NodeTypeLabel *child = dynamic_cast<NodeTypeLabel *>(childAt(event->pos()));
    if (!child) {
        //if there is no child at this position - return
        return;
    }

     /*
      * User pressed on a child
      * Mouse position relative to child top left corner
      */
    QPoint hotSpot = event->pos() - child->pos();

    QByteArray Data;
    QDataStream dataStream(&Data, QIODevice::WriteOnly);
    dataStream << QPoint(hotSpot);
    int id = 0;
    dataStream << id;
    dataStream << child->getType();


    //set the mime data for the drag
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("ANISE-GUI/drawobject", Data);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(*child->pixmap());
    drag->setHotSpot(hotSpot);
    this->setWindowOpacity(0.0);

    //determine the drag operation
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) ==
            Qt::MoveAction);
    else {
        // end of drag
        child->show();
    }
    drag->deleteLater();
}
