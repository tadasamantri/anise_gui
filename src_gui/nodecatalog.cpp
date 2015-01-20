#include "nodecatalog.h"

NodeCatalog::NodeCatalog() {}
void NodeCatalog::insert(Node *nodeToInsert) {

  this->Content.append(*nodeToInsert);
}
