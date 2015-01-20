#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QVector>
#include "connection.h"
#include "node.h"

class Mesh {
public:
  // add Node to mesh
  void addNode(Node *Node);

  // add Connection to mesh
  void addConnection(Connection *connection);

  // returns pointer to Node behind name
  Node *getNodeByName(QString name);

  // return node's address behind ID
  Node *getNodeByID(int ID);

  // return connection's address behind ID
  Connection *getConnectionByID(int ID);
  friend class JsonFilehandler;
  // private:
  //TODO nodes und connections als Map mit id
  QVector<Node *> nodes;
  QVector<Connection *> connections;
};

#endif // MESHHANDLER_H
