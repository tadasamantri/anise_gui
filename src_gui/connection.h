#ifndef CONNECTION_H
#define CONNECTION_H

#include "node.h"
#include "gate.h"

class Connection {
public:
  Connection(Gate &src, Gate &dest);
  Connection(Node &src, Node &dest);
  //TODO source_ndoe destination_Node source_Gate sourec_gate als konstruktor
  Gate *getSrcGate();
  Gate *getDestGate();
  void setSrcGate(Gate &src);
  void setDestGate(Gate &dest);
  Node *getSrcNode();
  Node *getDestNode();
  void setSrcNode(Node &node);
  void setDestNode(Node &node);

private:
  Gate *gateA, *gateB;
  Node *src_node, *dest_node;
};

#endif // CONNECTION_H
