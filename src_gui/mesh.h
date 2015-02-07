#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include "connection.h"

class Mesh : public QObject {
    Q_OBJECT

public:
    // konstruktor
    explicit Mesh(QObject *parent = 0);


    // add Node to mesh
    // return the id of the new node
    int addNode(Node *Node);

    // add Connection to mesh
    // return the id of the new connection
    int addConnection(Connection *connection);

    void addNodes(QList<Node *> &list);

    void removeNode(int ID);

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

    //deletingfunctions
    bool deleteItem();


    int getCurrentID();
public slots:

    void setFocusMeshObject(int nodeID);


private:


    // counter for ids.
    // prevents double id numbers
    int iDCounter;
    bool tableExists;
    // generate an unique id key;
    int generateId();

    int focusObject;

    //internal deletingfunctions
    bool deleteNode();
    bool deleteConnection();
};

#endif  // MESHHANDLER_H
