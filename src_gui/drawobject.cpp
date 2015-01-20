#include "drawobject.h"
#include <QPixmap>
#include <QWidget>

DrawObject::DrawObject(int numberOfNode, QWidget *parent = 0) {
  this->numberOfNode = numberOfNode;
  this->setParent(parent);
}
