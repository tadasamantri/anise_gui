#include "connection.h"

Connection::Connection(Node &src_node, Gate &src_gate, Node &dest_node,
                       Gate &dest_gate) {
  this->src_gate = &src_gate;
  this->dest_gate = &dest_gate;
  this->src_node = &src_node;
  this->dest_node = &dest_node;
}

void Connection::setDestGate(Gate &dest) { dest_gate = &dest; }

void Connection::setSrcGate(Gate &src) { src_gate = &src; }

Gate *Connection::getDestGate() { return dest_gate; }

Gate *Connection::getSrcGate() { return src_gate; }

void Connection::setDestNode(Node &node) { dest_node = &node; }

void Connection::setSrcNode(Node &node) { src_node = &node; }

Node *Connection::getSrcNode() { return src_node; }

Node *Connection::getDestNode() { return dest_node; }
