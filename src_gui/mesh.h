#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include "connection.h"
#include<QTableWidgetItem>

class Mesh : public QObject {
    friend class Data;
    friend class SingletonRender;
    friend class JSonFileHandler;

    Q_OBJECT
private:

    explicit Mesh(QObject *parent = 0);


    /**
     * @brief addNode Add a Node to the Mesh
     * @param Node The Node to be added
     * @return the ID of the added Node
     */
    int addNode(Node *Node);

    /**
     * @see addNode
     */
    int addConnection(Connection *connection);

    /**
     * @see addNode
     */
    int addNodes(QList<Node *> &list);

    /**
     * @brief removeNode
     * @param ID the ID of the Node to be removed
     */
    void removeNode(int ID);

    /**
     * @brief getNodeByName search a Node by its unique Name
     * @param name Name of the Node
     * @return Pointer to the Node
     */
    Node *getNodeByName(QString name);

    /**
     * @see getNodeByName
     */
    Node *getNodeByID(int ID);

    /**
     * @see getNodeByID()
     */
    Connection *getConnectionByID(int ID);


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
