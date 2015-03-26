#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QDir>
#include "mesh.h"
#include "nodecatalog.h"
#include "mainwindow.h"
#include "Render/singletonrender.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include "node.h"
#include "nodefactory.h"
#include "anisecommunicator.h"
#include <QTimer>

/**
 *  @Author Frederik Lührs
 *
 * Singleton class storing all data in one object (Mesh, nodes, node types,
 * important paths, ...)
 */
class Data : public QObject {
    Q_OBJECT

public:
    /**
    * The Mesh of user is currently working at
    */
    Mesh *mesh;

    /**
   * @brief nodeFactory
   * This will create any kind of Node and Node Templates.
   *
   */
    NodeFactory *nodeFactory;

    /**
    * The instance function returns a pointer to a static variable and thus is
    * declared static
    */
    static Data *instance();

    void initialize(MainWindow *mainWindow);

    /**
   * @brief getNodeCatalog
   * @return the NodeCatalog containing all types of nodes available
   */
    NodeCatalog *getNodeCatalog();

    /**
     * @brief addNodeToMesh inserts given node into the mesh and renders it afterwards
     * @param newNode
     * @return ID of inserted Node
     */
    int addNodeToMesh(Node *newNode);
    /**
     * @brief addConnectionToMesh inserts new Connection to mesh
     * @param newConnection
     * @return
     * @see addNodeToMesh
     */
    int addConnectionToMesh(Connection *newConnection);
    /**
     * @brief sortCircle
     * @see Mesh::sortCircle()
     */
    void sortCircle();
    /**
     * @brief sortRow
     * @see Mesh::sortRow()
     */
    void sortRow();
    /**
     * @brief sortForce
     * @see Mesh::sortForce()
     */
    void sortForce();

    int getFocusedID();
    /**
     * @brief removeNodeFromMesh removes node with given ID from mesh
     * @param ID
     */
    void removeNodeFromMesh(int ID);
    /**
     * @brief checkConnection verify if a connection is valid
     * @param srcNodeID
     * @param srcGate
     * @param destNodeID
     * @param destGate
     * @return
     */
    bool checkConnection(int srcNodeID, QString srcGate, int destNodeID, QString destGate);
    bool hasChanged();
    void unsetChanged();
    bool mustRepaint();
    void unsetRepaint();
    void setChanged();
    /**
     * @brief getNodeByName searches for node by name
     * @param name
     * @return
     */
    Node *getNodeByName(QString name);
    Node *getNodeByID(int ID);
    QList<Node *> getAllNodes();
    QList<Connection *> getAllConnections();
    QMap<int, Node *> *nodesInMesh();
    QString getValidAlternativeForName(const QString &name);
    /**
     * @brief moveObjectInMesh
     * @param Position
     * @param ID
     * @see moveObjectInMesh(QPoint*,QPoint*,int)
     */
    void moveObjectInMesh(QPoint *Position, int ID);
    /**
     * @brief moveObjectInMesh moves drwan object with given id from start to end
     * @param start start point
     * @param end end point
     * @param ID ID of moving item
     */
    void moveObjectInMesh(QPoint *start, QPoint *end, int ID);
    void finishMesh();
    void newMeshProject();

    Mesh *getMesh();

    MainWindow *getMainWindow() const;
    void setMainWindow(MainWindow *value);

    ~Data();
    void setEditMode();
    void setDrawLineMode(QString gateType);
    bool isSimulating();
    QList<Connection *> getConnections(const int &nodeID);

    QString getSaveFile() const;
    void setSaveFile(const QString &value);

    bool isRunning() const;

    void testChangeRun();

signals:

    void runModeChanged();

public slots:

    bool deleteItem();
    void setFocusMeshObject(int nodeID);
    void updateNode(QTableWidgetItem *item);
    void startSimulation();
    void stopSimulation();
    void runMesh();
private slots:
    /**
     * @brief autosaveMesh saves a backup file of the current mesh
     */
    void autosaveMesh();
private:
    explicit Data(QObject *parent = 0);

    bool runMode;

    /**
    * It is Private so that it can't be called.
    * Only the class function Instance can call the constructor.
    * Public access to the constructor is denied.
    *
    * The constructor, copy constructor and assignment operator are all private
    * to ensure that the programmer using the singleton class can only create a
    * single instance of the class using only the instance() function.
    *
    */

    // copy constructor does not need to be overridden because QObject doesnt has
    // a public copy instruktor

    // assignment constructor
    Data &operator=(Data const &) { return *this; }

    /**
    * Pointer to this instance
    */
    static Data *data;
    MainWindow *mainWindow;
    /**
    * Contains all Node types
    * (Unique list of node objects of all types)
    */
    NodeCatalog *nodeCatalog;
    void updateConnections(int ID, QPoint offset);
    bool changed = false;
    bool repaint = false;
    bool onSimulation;
    //timer for autobackups
    QTimer *backupTimer;
    //directory for savings
    QDir autosave;
    //location of current save file
    QString saveFile;
    AniseCommunicator *framework;
    //interval to create backup files in ms
    int autosave_interval = 3e5;
    //location of last made backup
    QString lastBackupFile;
};

#endif  // DATA_H
