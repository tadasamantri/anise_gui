#include "drawobject.h"
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QShortcut>
#include <QDebug>
#include <QVector>
#include <QBitmap>
#include <QSize>


//#include "data.h"

DrawObject::DrawObject(int nodeID, QPoint position, QWidget *parent = 0) {


   // qDebug() << "DRAWOBJECT CREATED. ID: " << nodeID;
   // qDebug() << this;
    this->nodeID = nodeID;
    this->setParent(parent);


    mask = QBitmap(this->size());
    mask.fill(Qt::white);


   // allOverPic= QPixmap(QSize(100,100));
   // painter = QPainter(&allOverPic);

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
void DrawObject::addPicture(QPixmap *pic, QPoint position, int width, int height) {

    QLabel *label = new QLabel(this);
    label->setPixmap(*pic);
    //this->setMask(pic->mask());
    label->setMask(pic->mask());

    this->labelvector.append(label);
    label->setGeometry(position.x(), position.y(), width, height);



   //  Something to build your own mask

   QPainter painter(&mask);
   painter.setBrush(Qt::black);
   painter.drawPixmap(position.x(), position.y(), width, height, *pic);

   qDebug() << mask.colorCount();
   //painter.~QPainter();


   // Draw the final image.
   this->setMask(mask);





}

void DrawObject::show(){




    QWidget::show();
/*
    foreach (QLabel *label, this->labelvector) {
        qDebug()<<"trying to show";
        label->show();
    }*/

}

/*
void addMask(QBitmap map){

    map.


}*/
