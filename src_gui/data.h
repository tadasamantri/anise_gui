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
   * @brief Data::getNodeCatalog
   * @return the NodeCatalog containing all types of nodes available
   */
    NodeCatalog *getNodeCatalog();

    /**
     * @brief addNodeToMesh adds given Node into the Mesh
     * @param newNode
     * @return ID of added Node
     */
    int addNodeToMesh(Node *newNode);
    /**
     * @see addNodeToMesh
     */
    int addNode(Node *node);
    int addConnectionToMesh(Connection *newConnection);
    int addConnection(Connection *newConnection);
    void sortCircle();
    void sortRow();
    void sortForce();
    void runMesh();
    int getFocusedID();
    void removeNodeFromMesh(int ID);
    bool checkConnection(int srcNodeID, QString srcGate, int destNodeID, QString destGate);
    bool hasChanged();
    void unsetChanged();
    bool mustRepaint();
    void unsetRepaint();
    void setChanged();
    Node *getNodeByName(QString name);
    Node *getNodeByID(int ID);
    QList<Node *> getAllNodes();
    QList<Connection *> getAllConnections();
    QMap<int, Node *> *nodesInMesh();
    QString getValidAlternativeForName(QString &name);
    void moveObjectInMesh(QPoint *Position, int ID);
    void moveObjectInMesh(QPoint *start, QPoint *end, int ID);

    void newMeshProject();

    Mesh *getMesh();

    MainWindow *getMainWindow() const;
    void setMainWindow(MainWindow *value);

    ~Data();
    void setEditMode();
    void setDrawLineMode(QString gateType);
    bool isSimulating();
    QList<Connection *> getConnections(int nodeID);

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
