#include "testdrawobject.h"
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QShortcut>
#include <qdebug.h>

//#include "data.h"

TestDrawObject::TestDrawObject(int nodeID, QPoint position, QWidget *parent = 0) {


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
void TestDrawObject::addPicture(QPixmap *pic, QPoint position) {

    QLabel *label = new QLabel(this);

    qDebug() << "dummy läuft" << position;

    label->setPixmap(*pic);

    qDebug() << "dummy läuft";

    label->setGeometry(0, 0, 70, 70);

    this->show();






}

