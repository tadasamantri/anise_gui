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
     * @brief removeNode
     * @param ID the ID of the Node to be removed
     */
    void deleteNode(const int &ID);

    /**
     * @brief getNodeByName search a Node by its unique Name
     * @param name Name of the Node
     * @return Pointer to the Node
     */
    Node *getNodeByName(const QString &name);

    /**
     * @see getNodeByName
     */
    Node *getNodeByID(const int &ID);

    /**
     * @see getNodeByID()
     */
    Connection *getConnectionByID(const int &ID);

    /**
     * @brief getAllNodes
     * @return a collection of all nodes inside the mesh
     */
    QList<Node *> getAllNodes();

    /**
     * @brief getAllConnections
     * @see getAllNodes
     */
    QList<Connection *> getAllConnections();

    // Nodes and connections
    QMap<int, Node *> nodesInMesh;
    QMap<int, Connection *> connectionsInMesh;

    /**
     * @brief getFocusedNode
     * @return the node in focus
     */
    Node* getFocusedNode();
    //deletingfunctions
    bool deleteItem();
    /**
     * @brief validName
     * @param name
     * @return true if given name is a valid (unique and non-empty) node name
     */
    bool validName(const QString &name);
    /**
     * @brief getValidAlternativeForName checks if given name is valid and generates a valid alternative
     * @param name
     * @return valid alternative for name (may be the name itself!)
     */
    QString getValidAlternativeForName(const QString name);

    void sortCircle();
    void sortRow();
    void sortForce();

    ~Mesh();
    /**
     * @brief checkConnection checks if a connection between given nodes and gates is valid
     * @param srcNodeID
     * @param srcGate
     * @param destNodeID
     * @param destGate
     * @return
     */
    bool checkConnection(const int &srcNodeID, const QString &srcGate, const int &destNodeID, const QString &destGate);
signals:
    void changed();
public slots:
    void setFocusMeshObject(const int &nodeID);
    void updateNode(QTableWidgetItem *item);
    void checkIfExecutable();
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

QList<Connection *> getConnectionsToNode(const int &nodeID);


    //internal deletingfunctions
    //if no parameters given ID is taken from focusObject!!
    bool deleteNode();


    bool deleteConnection();
    bool deleteConnection(const int &conToDeleteID);
    bool deleteConnection(Connection *c);
};

#endif  // MESHHANDLER_H
