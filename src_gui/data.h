#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "mesh.h"
#include "nodecatalog.h"
#include "mainwindow.h"
#include "singletonrender.h"
#include "ui_mainwindow.h"

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

    int addNodeToMesh(Node *newNode);
    void removeNodeFromMesh(int ID);
    void moveNodeInMesh(QPoint *Position, int numberOfNode);

    void newMeshProject();

    Mesh *getMesh();



public slots:
    bool deleteItem();

private:
    explicit Data(QObject *parent = 0);

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

    //copy constructor does not need to be overridden because QObject doesnt has a public copy instruktor


    // assignment constructor
    Data &operator=(Data const &){

        return *this;

    };

    /**
      * Pointer to this instance
      */
    static Data *data;

    /**
      * Contains all Node types
      * (Unique list of node objects of all types)
      */
    NodeCatalog *nodeCatalog;
};

#endif  // DATA_H
