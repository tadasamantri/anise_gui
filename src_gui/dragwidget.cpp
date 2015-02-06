#include "dragwidget.h"
#include <QMouseEvent>
#include <QPoint>
#include <QDrag>
#include <QDebug>
#include <Qt>
#include <QMimeData>
#include <QIODevice>
#include <QPixmap>
#include <QLabel>
#include "drawobject.h"
#include <QPainter>
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

    NodeTypeLabel *child = static_cast<NodeTypeLabel *>(childAt(event->pos()));
    if (!child) {
        /**
          * If no child at this position - return
          */
        return;
    }

    /**
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



    /**
      * Something about mime data
      */
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/customthingy", Data);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(*child->pixmap());
    drag->setHotSpot(hotSpot);
    this->setWindowOpacity(0.0);
    /*
   *
   * //TODO in die render klasse damit!
     // This will change the color of the drag icon; could also be used to
     //generate new images
        QPixmap *tempPixmap = *(child->pixmap());
        QPainter painter;
        painter.begin(tempPixmap);
        painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
        painter.end();

    // child->setPixmap(tempPixmap);
  */

    // TODO rausinfdne was das hier macht und der gruppe erklären
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) ==
            Qt::MoveAction)
        qDebug() << "never happens?";
    // child->close();
    else {
        // qDebug() << "drag end";
        child->show();
        // child->setPixmap(pixmap);
    }
}
