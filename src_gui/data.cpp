#include "data.h"
#include "singletonrender.h"
#include "ui_mainwindow.h"

// Global static pointer used to ensure a single instance of the class.
Data *Data::data = NULL;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

Data *Data::instance() {
  if (!data)  // Only allow one instance of class to be generated.
    data = new Data;

  return data;
}

Data::Data(QObject *parent) : QObject(parent) {
  mesh = new Mesh();
  nodeCatalog = new NodeCatalog();
}

void Data::addNodeToMesh(Node *newNode) {
  this->mesh->addNode(newNode);
  SingletonRender::instance()->renderMesh(this->mesh);
}

void Data::moveNodeInMesh(QPoint *Position, int numberOfNode) {
  this->mesh->getNodeByID(numberOfNode)
      ->setPosition(Position->x(), Position->y());
  SingletonRender::instance()->renderMesh(this->mesh);
}
