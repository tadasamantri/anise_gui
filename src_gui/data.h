#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "mesh.h"
#include "nodecatalog.h"
#include "mainwindow.h"
#include "data.h"
#include "singletonrender.h"
#include "ui_mainwindow.h"

/*
 *Author: Frederik Lührs
 *
 * Singleton class storing all data in one object (Mesh, nodes, node types,
 *important paths, ...)
 *
 */
class Data : public QObject {
    Q_OBJECT
public:
    // Mesh the user currently works at
    Mesh *mesh;

    // Contains all Node types (unique list of node objects of all types)
    NodeCatalog *nodeCatalog;

    // The instance function returns a pointer to a static variable and thus is
    // declared static.
    static Data *instance();

    void addNodeToMesh(Node *newNode);
    void moveNodeInMesh(QPoint *Position, int numberOfNode);

private:
    explicit Data(QObject *parent = 0);

    // Private so that it can  not be called
    // Only the class function Instance can call the constructor. Public access to
    // the constructor is denied.
    // Data();

    // The constructor, copy constructor and assignment operator are all private
    // to ensure that the
    // programmer using the singleton class can only create a single instance of
    // the class using only the Instance() function.

    // copy constructor is private
    Data(Data const &){};

    // assignment operator is private
    Data &operator=(Data const &){};

    // pointer to this instance
    static Data *data;
};

#endif  // DATA_H
