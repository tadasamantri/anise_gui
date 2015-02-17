#include "connection.h"
#include <limits>
#include "data.h"

Connection::Connection(Node *src_node, Gate *src_gate, Node *dest_node,
                       Gate *dest_gate, QVector<QPoint> waypoints) {
    this->src_gate = src_gate;
    this->dest_gate = dest_gate;
    this->src_node = src_node;
    this->dest_node = dest_node;
    this->waypoints = waypoints;
}

Connection::Connection(int srcNodeID, QString srcGateName, int destNodeID, QString destGateName, QVector<QPoint> waypoints)

    : Connection(
                    Data::instance()->getMesh()->getNodeByID(srcNodeID),
                    Data::instance()->getMesh()->getNodeByID(srcNodeID)->getGateByName(srcGateName),
                    Data::instance()->getMesh()->getNodeByID(destNodeID),
                    Data::instance()->getMesh()->getNodeByID(destNodeID)->getGateByName(destGateName),
                    waypoints
    ){}


Connection::Connection(){}



//will return the closest joint to this point
int Connection::getJoint(QPoint* point){

    if(this->waypoints.isEmpty()){
        qDebug() << "tried to get a joint from a connection without joints!";
        return -1;
    }

    //uses manhatten length (x+y) instead of correct pytharorean calculations
    int min = std::numeric_limits<int>::max();
    int closestOne = -1;
    for(int index = 0; index < this->waypoints.size(); ++index) {
        QPoint tempPoint(point->x()-this->waypoints.at(index).x(),point->y()-this->waypoints.at(index).y() );
        int manhattenLength = tempPoint.manhattanLength();
        qDebug() << "manhatten length: " << manhattenLength << " min: " << min << " id: " << index;
        if (manhattenLength < min) {
            min = manhattenLength;
            closestOne = index;
        }
    }
    qDebug()<< "closestone " << closestOne;
    return closestOne;
}

void Connection::setJoint(int index, QPoint *newPosition){

    waypoints.removeAt(index);
    waypoints.insert(index, *newPosition);

}

void Connection::setDestGate(Gate *dest) { dest_gate = dest; }

void Connection::setSrcGate(Gate *src) { src_gate = src; }

Gate *Connection::getDestGate() { return dest_gate; }

Gate *Connection::getSrcGate() { return src_gate; }

void Connection::setDestNode(Node *node) { dest_node = node; }
QVector<QPoint> Connection::getWaypoints() const
{
    return waypoints;
}

void Connection::setWaypoints(const QVector<QPoint> &value)
{
    waypoints = value;
}


void Connection::setSrcNode(Node *node) { src_node = node; }

Node *Connection::getSrcNode() { return src_node; }

Node *Connection::getDestNode() { return dest_node; }
