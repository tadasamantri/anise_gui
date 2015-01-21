#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QVector>
#include <QMap>
#include <QString>
#include <QList>

#include "connection.h"
#include "node.h"

class Mesh {
    friend class JsonFilehandler;

public:
    // konstruktor
    Mesh();

    // add Node to mesh
    // return the id of the new node
    int addNode(Node *Node);

    // add Connection to mesh
    // return the id of the new connection
    int addConnection(Connection *connection);

    // returns pointer to Node behind name
    Node *getNodeByName(QString name);

    // return node's address behind ID
    Node *getNodeByID(int ID);

    // return connection's address behind ID
    Connection *getConnectionByID(int ID);

    // returns all Nodes from nodesInMash as a QList
    QList<Node *> getAllNodes();

    // returns all Connection from connectionsInMash as a QList
    QList<Connection *> getAllConnections();

    // Nodes and connections
    QMap<int, Node *> nodesInMash;
    QMap<int, Connection *> connectionsInMash;

private:
    // counter for ids.
    // prevents double id numbers
    int iDCounter;

    // generate an unique id key;
    int generateId();
};

#endif  // MESHHANDLER_H
