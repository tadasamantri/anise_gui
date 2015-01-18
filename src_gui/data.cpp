#include "data.h"

Data::Data(QObject *parent) :
    QObject(parent)
{

    mesh = new Mesh();
    nodeCatalog = new NodeCatalog();
}
