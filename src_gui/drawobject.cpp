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

    this->nodeID = nodeID;
    this->setParent(parent);

    // We say the constructor which position he has
    this->setGeometry(position.x(), position.y(), 100, 100);

    //initialize mainMask with complete Transparency
    mainMask = QBitmap(this->size());
    mainMask.fill(Qt::color0);
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


    //Instanstiate the drawing to be shown
    QLabel *label = new QLabel(this);
    label->setGeometry(position.x(), position.y(), width, height);
    label->setPixmap(*pic);
    this->labelvector.append(label);
    
    //Instanstiate the non-transparent mask with size of pic
    //QBitmap picMask(width, height);     //This is a mask manually produced with size of picture
    //picMask.fill(Qt::color1);
    QBitmap picMask = pic->mask();    //This is used to get the mask automatically from the picture but doesnt work all the time

    //Include the picture (non-transparent) to the main Mask to get the area of the picture to be visible
    QPainter painter(&mainMask);
    painter.setBrush(Qt::color1);
    painter.drawPixmap(position.x(), position.y(), width, height, picMask);

    //Now set the masks to the widgets
    label->setMask(pic->mask());
    this->setMask(mainMask);


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
