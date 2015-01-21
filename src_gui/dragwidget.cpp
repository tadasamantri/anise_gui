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

DragWidget::DragWidget(QWidget *parent) : QWidget(parent) {}

void DragWidget::mousePressEvent(QMouseEvent *event) {
    // user pressed the mouse button
    // ensure a left-mouse-click
    if (!(event->button() == Qt::LeftButton)) {
        qDebug() << "that was not the left mouse button!\n";
        return;
    }

    QLabel *child = static_cast<QLabel *>(childAt(event->pos()));
    if (!child) {
        // if no child at this position return
        return;
    }

    // user pressed on a child
    // mouse position relative to child top left corner
    QPoint hotSpot = event->pos() - child->pos();

    QByteArray Data;
    QDataStream dataStream(&Data, QIODevice::WriteOnly);
    dataStream << QPoint(hotSpot);

    // something about mime data...
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/customthingy", Data);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(*child->pixmap());
    drag->setHotSpot(hotSpot);

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
