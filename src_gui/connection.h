#ifndef CONNECTION_H
#define CONNECTION_H

#include "node.h"
#include "gate.h"

class Connection
{
public:
    Connection(Gate &src, Gate &dest);
    Connection(Node &src, Node &dest);
    Gate* getSrcGate();
    Gate* getDestGate();
    void setSrcGate(Gate &src);
    void setDestGate(Gate &dest);
    //return true if validConnection(Node &src, &dest); Connection is made
    bool isValid();
    Node* getSrcNode();
    Node* getDestNode();
    void setSrcNode(Node &node);
    void setDestNode(Node &node);


private:
        Gate *gateA, *gateB;
        Node *src_node, *dest_node;

};

#endif // CONNECTION_H
