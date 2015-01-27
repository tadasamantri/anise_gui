#include "drawobject.h"
#include <QPixmap>
#include <QWidget>
#include <QShortcut>

#include "data.h"

DrawObject::DrawObject(int nodeID, QWidget *parent = 0) {

    //new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(deleteItem(nodeID)));

   QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
   connect(shortcut, SIGNAL(activated()), Data::instance(), SLOT(deleteItem()));

    this->nodeID = nodeID;
    this->setParent(parent);
}




