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

    //initialize the over all picture
    this->overAllPicture = QPixmap (this->size());
    overAllPicture.fill(QColor(127,127,127,0));
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
    int height = pic->height();
    int width = pic->width();

    //Instanstiate the drawing to be shown
    QLabel *label = new QLabel(this);
    label->setGeometry(position.x(), position.y(), width, height);
    label->setPixmap(*pic);
    this->labelvector.append(label);

    //Instanstiate the non-transparent mask with size of pic
    //QBitmap picMask(width, height);     //This is a mask manually produced with size of picture
    //picMask.fill(Qt::color1);
    QBitmap picMask = pic->createMaskFromColor(Qt::magenta);    //This is used to get the mask automatically from the picture but doesnt work all the time

    //Include the picture (non-transparent) to the main Mask to get the area of the picture to be visible
    QPainter painter(&mainMask);
    painter.setBrush(Qt::color1);
    painter.drawPixmap(position.x(), position.y(), width, height, picMask);

    //Now set the masks to the widgets
    label->setMask(pic->mask());
    this->setMask(mainMask);

    //update the over all picture
    // thats the one used for dragging etc.
    this->updateOverAllPicture(pic, position);


}

void DrawObject::updateOverAllPicture(QPixmap *newPicture, QPoint position ){
    //save the dimension of the new picture
    int height = newPicture->height();
    int width = newPicture->width();

    newPicture->setMask(newPicture->createMaskFromColor(Qt::magenta));

    //create a painter
    //newPicture->setMask(mainMask);
    QPainter painter(&(this->overAllPicture));
    painter.setBrush(Qt::black);
    painter.drawPixmap(position.x(), position.y(), width, height, *newPicture);



//reateMaskFromColor(Qt::magenta);

    //de struktor!!
    //~painter;


}

QPixmap DrawObject::getPicture(){

    return this->overAllPicture;

}

