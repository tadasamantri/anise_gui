#ifndef SINGLETONRENDER_H
#define SINGLETONRENDER_H

// Qt classes
#include <QMap>
#include <QPixmap>
#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QList>
#include <QPainter>

// our own classes
#include "mesh.h"
#include "node.h"
#include "drawobject.h"
#include "ui_mainwindow.h"

// The life of the singleton instantiation is for the duration of the
// application.

class SingletonRender {
public:
    // The instance function returns a pointer to a static variable and thus is
    // declared static.
    static SingletonRender *instance();

    // will load all images in ../Data/Images/ into the allImages Map
    bool loadImages();

    void setUi(Ui::MainWindow *ui);

    /*
   * Renders the whole mesh
   */
    void renderMesh(Mesh *workMesh);

    /*
     * Clears the MeshField
     */
    void clearMeshField();

    bool deleteMeshDrawing(int objectID);


    /*
     *Testfunction of Deleting
     * Returns all ID's of given widget
     */
    QVector<int> *getChildrenIDs();

    /*
   * Renders a node.
   *
   */
    void renderNode(Node *nodeToRender, int nodeID);

    /*
   * Render one Node Type in the Catalog
   *
   */
    void renderNodeType(Node *nodeToRender, QWidget *parent, int position);

    /*
   * This will Render all Nodes in the Type Catalog.
   */
    void renderCatalogContent(QVector<Node> NodeVektor);

    /*
     * Draws a line from start x y to end x y
     *
     */
    void drawLine(double start_x, double start_y, double end_x ,double end_y);

private slots:
    void on_actionNew_triggered();

private:

    void paintEvent(QPaintEvent *event);

    void clearAll(QWidget *parent); // Deletes all children of given parent

    QPainter painter;

    // this is the map containing all raw Images.
    // Qstring is the same as the filename
    // QPixmap a .png image
    QMap<QString, QPixmap *> allImages;

    // Maps containing everything that have been rendered
    // int ID
    // and the widget
    //TODO WHY do we need This??


    //TODO JOIN BOTH LISTS TO ONE
    QMap<int, DrawObject *> allDrawnNodes;
    QMap<int, QLine *> allDrawnLines;

    // window of application
    Ui::MainWindow *ui;

    //###### singleton stuff ########

    // Private so that it can  not be called
    // Only the class function Instance can call the constructor. Public access to
    // the constructor is denied.
    SingletonRender();

    // The constructor, copy constructor and assignment operator are all private
    // to ensure that the
    // programmer using the singleton class can only create a single instance of
    // the class using only the Instance() function.

    // copy constructor is private
    SingletonRender(SingletonRender const &){};

    // assignment operator is private
    SingletonRender &operator=(SingletonRender const &){};

    // pointer to this instance
    static SingletonRender *m_pInstance;
};

#endif  // SINGLETONRENDER_H
