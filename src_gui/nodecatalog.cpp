#include "nodecatalog.h"

NodeCatalog::NodeCatalog() {}
void NodeCatalog::insert(Node *nodeToInsert) {
//TODO should be a map with "type", node
  this->Content.append(*nodeToInsert);
}
