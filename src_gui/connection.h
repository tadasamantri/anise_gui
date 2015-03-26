#ifndef CONNECTION_H
#define CONNECTION_H

#include "node.h"
#include "gate.h"

#include <QPoint>
#include <QDebug>

class Connection {
public:
    Connection();
    Connection(Node *srcNode, Gate *srcGate, Node *destNode, Gate *destGate,
               QVector<QPoint> joints = QVector<QPoint>());
    Connection(int srcNodeID, QString srcGateName, int destNodeID,
               QString destGateName,
               QVector<QPoint> joints = QVector<QPoint>());

    /**
     * @brief getJoint
     * @param point
     * @return closest joint to given point
     */
    int getJoint(QPoint *point);
    /**
     * @brief setJoint set joint with given to new position
     * @param index index of joint
     * @param newPosition the new position
     */
    void setJoint(int index, QPoint *newPosition);

    Gate *getSrcGate();
    Gate *getDestGate();

    void setSrcGate(Gate *src);
    void setDestGate(Gate *dest);

    Node *getSrcNode();
    Node *getDestNode();

    void setSrcNode(Node *node);
    void setDestNode(Node *node);

    QVector<QPoint> *getJoints();
    void setJoints(const QVector<QPoint> &value);
    /**
     * @brief clearJoints deletes all joints
     */
    void clearJoints();
    int getID() const;
    void setID(int value);

private:
    Gate *srcGate;
    Gate *destGate;
    Node *srcNode;
    Node *destNode;
    QVector<QPoint> joints;
    int ID;
};

#endif  // CONNECTION_H
