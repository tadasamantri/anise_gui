#include "mesh.h"



void Mesh::addNode(Node &node){
    this->nodes << &node;
}
