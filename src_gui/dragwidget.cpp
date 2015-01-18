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

DragWidget::DragWidget(QWidget *parent) :
    QWidget(parent)
{
}

void DragWidget::mousePressEvent(QMouseEvent *event){
    qDebug() << "click";
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if(!child){
        qDebug() << "no child";
        return;
    }

    //ensure a left-mouse-click
    if(!(event->button() == Qt::LeftButton)){
        qDebug() << "that was not the left mouse button!\n";
        return ;
    }

    qDebug() << "child";
    QPoint hotSpot = event->pos() - child->pos();

    QByteArray Data ;
    QDataStream dataStream(&Data, QIODevice::WriteOnly);
    dataStream << child << QPoint(hotSpot);

    //something about mime data...
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/customthingy", Data);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(*child->pixmap());
    drag->setHotSpot(hotSpot);
/*
 // This will change the color of the drag icon; could also be used to generate new images
    QPixmap tempPixmap = child->pixmap();
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();
*/
   // child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
        qDebug() << "never happens?";
        //child->close();
    else {
        qDebug() << "drag end";
     child->show();
     //child->setPixmap(pixmap);
    }

}




