#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include "connection.h"
#include<QTableWidgetItem>

class Mesh : public QObject {
    friend class Data;
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

    int addNodes(QList<Node *> &list);

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
    QMap<int, Node *> nodesInMesh;
    QMap<int, Connection *> connectionsInMesh;

    Node* getFocusedNode();
    //deletingfunctions
    bool deleteItem();
    bool validName(const QString &name);

    int getCurrentID();
    QString getValidAlternativeForName(const QString name);

    void sortCircle();
    void sortRow();
    void sortForce();

    ~Mesh();
    bool checkConnection(int srcNodeID, QString srcGate, int destNodeID, QString destGate);
public slots:
    void setFocusMeshObject(int nodeID);
    void updateNode(QTableWidgetItem *item);

private:


    // counter for ids.
    // prevents double id numbers
    int iDCounter;
    bool tableExists;
    // generate an unique id key;
    int generateId();

    //it is very important only chinging focusObject by using setFocusMeshObject!!
    int focusObject;

    //update Connection start and ende
    void updateConnStartAndEnd();

QList<Connection *> getConnectionsToNode(int nodeID);


    //internal deletingfunctions
    //if no parameters given ID is taken from focusObject!!
    bool deleteNode();


    bool deleteConnection();
    bool deleteConnection(int conToDeleteID);
    bool deleteConnection(Connection *c);
};

#endif  // MESHHANDLER_H
