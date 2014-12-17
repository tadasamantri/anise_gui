#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QVector>
#include "connection.h"
#include "node.h"


class Mesh
{
public:

    //add Node to mesh
    void addNode(Node &Node);

    //add Connection to mesh
    void addConnection();

    //return node's address behind ID
    Node* getNodeByID(int ID);

    //return connection's address behind ID
    Connection* getConnectionByID(int ID);

private:
    QVector<Node*> nodes;
    QVector<Connection*> connections;

};

#endif // MESHHANDLER_H
