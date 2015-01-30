#include "drawobject.h"
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QShortcut>
#include <QDebug>
#include <QVector>
#include <QBitmap>

//#include "data.h"

DrawObject::DrawObject(int nodeID, QPoint position, QWidget *parent = 0) {


   // qDebug() << "DRAWOBJECT CREATED. ID: " << nodeID;
   // qDebug() << this;
    this->nodeID = nodeID;
    this->setParent(parent);

    // We say the constructor which position he has
    this->setGeometry(position.x(), position.y(), 100, 100);

}

/**
 * Has two parameter for the widget.
 * We create label object.
 * we give the label the pixmap and the geometry.
 * Then we show it.
 *
 * @brief TestDrawObject::addPicture
 * @param pic       picture which you want to add
 * @param position  position relativ to top left corner
 */
void DrawObject::addPicture(QPixmap *pic, QPoint position) {

    QLabel *label = new QLabel(this);
    label->setPixmap(*pic);
    this->setMask(pic->mask());

    qDebug() << "PIXMAP: " << pic->size();
    qDebug() << "BITMAP: " << pic->mask();
    label->setMask(pic->mask());
    this->labelvector.append(label);
    label->setGeometry(0, 0, 50, 50);


    /*
     Something to build your own mask

     Draw the mask.
   QBitmap  mask(original.size());
   QPainter painter(&mask);
   mask.fill(Qt::white);
   painter.setBrush(Qt::black);
   painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), 100, 100);

   // Draw the final image.
   original.setMask(mask);
*/
    //this->show();

}

void DrawObject::show(){




    QWidget::show();
/*
    foreach (QLabel *label, this->labelvector) {
        qDebug()<<"trying to show";
        label->show();
    }*/

}
