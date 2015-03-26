#include "connection.h"
#include <limits>
#include "data.h"

Connection::Connection(Node *src_node, Gate *src_gate, Node *dest_node,
                       Gate *dest_gate, QVector<QPoint> waypoints) {
  this->srcGate = src_gate;
  this->destGate = dest_gate;
  this->srcNode = src_node;
  this->destNode = dest_node;
  this->joints = waypoints;
}

Connection::Connection(int srcNodeID, QString srcGateName, int destNodeID,
                       QString destGateName, QVector<QPoint> waypoints)

    : Connection(
          Data::instance()->getNodeByID(srcNodeID),
          Data::instance()->getNodeByID(srcNodeID)->getGateByName(
              srcGateName),
          Data::instance()->getNodeByID(destNodeID),
          Data::instance()->getNodeByID(destNodeID)->getGateByName(
              destGateName),
          waypoints) {}

Connection::Connection() {}

/**
 * @brief Connection::getJoint
 * @param point
 * @return ID of closest joint to given point
 */
int Connection::getJoint(QPoint *point) {
  if (this->joints.isEmpty()) {
    qDebug() << "tried to get a joint from a connection without joints!";
    return -1;
  }

  // uses manhatten length (x+y) instead of correct pythagorean calculations
  int min = std::numeric_limits<int>::max();
  int closestOne = -1;
  for (int index = 0; index < this->joints.size(); ++index) {
    QPoint tempPoint(point->x() - this->joints.at(index).x(),
                     point->y() - this->joints.at(index).y());
    int manhattenLength = tempPoint.manhattanLength();
    if (manhattenLength < min) {
      min = manhattenLength;
      closestOne = index;
    }
  }
  return closestOne;
}
/**
 * @brief Connection::setJoint
 * @param index index of joint
 * @param newPosition position to move the joint to
 */
void Connection::setJoint(int index, QPoint *newPosition) {
  joints.removeAt(index);
  joints.insert(index, *newPosition);
}

void Connection::setDestGate(Gate *dest) { destGate = dest; }

void Connection::setSrcGate(Gate *src) { srcGate = src; }

Gate *Connection::getDestGate() { return destGate; }

Gate *Connection::getSrcGate() { return srcGate; }

void Connection::setDestNode(Node *node) { destNode = node; }

QVector<QPoint> *Connection::getJoints() { return &joints; }

void Connection::setJoints(const QVector<QPoint> &value) {  joints = value; }

void Connection::clearJoints()
{
    joints.clear();
    SingletonRender::instance()->clearAllConnections();
}

int Connection::getID() const { return ID; }

void Connection::setID(int value) { ID = value; }

void Connection::setSrcNode(Node *node) { srcNode = node; }

Node *Connection::getSrcNode() { return srcNode; }

Node *Connection::getDestNode() { return destNode; }
