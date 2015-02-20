#ifndef CONNECTION_H
#define CONNECTION_H

#include "node.h"
#include "gate.h"

#include <QPoint>
#include <QDebug>

class Connection {

public:
    Connection();
    Connection(Node *src_node, Gate *src_gate, Node *dest_node, Gate *dest_gate, QVector<QPoint> waypoints = QVector<QPoint>());
    Connection(int srcNodeID, QString srcGateName, int destNodeID, QString destGateName, QVector<QPoint> waypoints = QVector<QPoint>());


    //returns the closest joint to the point
    int getJoint(QPoint *point);
    void setJoint(int index, QPoint *newPosition);

    Gate *getSrcGate();
    Gate *getDestGate();

    void setSrcGate(Gate *src);
    void setDestGate(Gate *dest);

    Node *getSrcNode();
    Node *getDestNode();

    void setSrcNode(Node *node);
    void setDestNode(Node *node);

    QVector<QPoint> waypoints;

    QVector<QPoint> getWaypoints() const;
    void setWaypoints(const QVector<QPoint> &value);

    int getID() const;
    void setID(int value);

private:
    Gate *src_gate, *dest_gate;
    Node *src_node, *dest_node;

    int ID;
};

#endif  // CONNECTION_H
