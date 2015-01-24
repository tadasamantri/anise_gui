#include "drawobject.h"
#include <QPixmap>
#include <QWidget>
#include <QShortcut>

//#include "data.h"

DrawObject::DrawObject(int nodeID, QWidget *parent = 0) {

    //new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(deleteItem(nodeID)));

   // QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), parent);
   // connect(shortcut, SIGNAL(activated()), this, SLOT(deleteItem()));

    this->nodeID = nodeID;
    this->setParent(parent);
}
/*
void DrawObject::deleteItem(){

    qDebug() << "O GOT DELETED" << this;

}
*/

