#include "connection.h"

Connection::Connection(QObject *parent) :
    QObject(parent)
{
}

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
