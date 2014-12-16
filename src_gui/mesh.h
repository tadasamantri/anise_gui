#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QObject>
#include <QVector>
#include "connection.h"
#include "node.h"


class Mesh
{
public:

    //add Node to mesh
    void addNode(const Node &Node);

    //add Connection to mesh
    void addConnection();

    //return node's address behind ID
    Node* getNodeByID(int ID);

    //return connection's address behind ID
    Connection* getConnectionByID(int ID);

signals:

public slots:

};

#endif // MESHHANDLER_H
