#include "connection.h"



Connection::Connection(Gate &src, Gate &dest)
{
    gateA = &src;
    gateB = &dest;
}

void Connection::setDestGate(Gate &dest)
{
    gateB = &dest;
}

void Connection::setSrcGate(Gate &src)
{
    gateA = &src;
}

Gate* Connection::getDestGate(){
    return gateB;
}

Gate*  Connection::getSrcGate(){
    return gateA;
}

void Connection::setDestNode(Node &node){
    dest_node = &node;
}

void Connection::setSrcNode(Node &node){
    src_node = &node;
}

Node* Connection::getSrcNode(){
    return src_node;
}

Node* Connection::getDestNode(){
    return dest_node;
}
