#include "drawobject.h"
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QShortcut>
#include <QDebug>
#include <QVector>

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
    this->labelvector.append(label);
    label->setGeometry(0, 0, 10, 10);

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
