#include "drawobject.h"
#include <QPixmap>
#include <QWidget>

DrawObject::DrawObject(int nodeID, QWidget *parent = 0) {
    this->nodeID = nodeID;
    this->setParent(parent);
}
