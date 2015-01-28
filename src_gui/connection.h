#ifndef CONNECTION_H
#define CONNECTION_H

#include "node.h"
#include "gate.h"

class Connection {

public:
    Connection(Node *src_node, Gate *src_gate, Node *dest_node, Gate *dest_gate);
    Gate *getSrcGate();
    Gate *getDestGate();
    void setSrcGate(Gate &src);
    void setDestGate(Gate &dest);
    Node *getSrcNode();
    Node *getDestNode();
    void setSrcNode(Node &node);
    void setDestNode(Node &node);

private:
    Gate *src_gate, *dest_gate;
    Node *src_node, *dest_node;
};

#endif  // CONNECTION_H
