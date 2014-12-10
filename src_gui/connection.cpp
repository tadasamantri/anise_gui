#include "connection.h"

Connection::Connection(QObject *parent) :
    QObject(parent)
{
}

Connection::Connection(Node &src, Node &dest)
{
    nodeA = &src;
    nodeB = &dest;
}

void Connection::setDestNode(Node &dest)
{
    nodeB = &dest;
}

void Connection::setSrcNode(Node &src)
{
    nodeA = &src;
}

Node* Connection::getDestNode(){
    return nodeB;
}

Node*  Connection::getSrcNode(){
    return nodeA;
}

Connection::~Connection()
{
    delete this;
}
