#ifndef SINGLETONRENDER_H
#define SINGLETONRENDER_H

//Qt classes
#include <QMap>
#include <QPixmap>
#include <QDebug>
#include <QStringList>
#include <QDir>

//our own classes
#include "mesh.h"
#include "node.h"
#include "drawobject.h"



//The life of the singleton instantiation is for the duration of the application.

class singletonRender
{
public:
    // this is the map containing all raw Images.
    // Qstring is the same as the filename
    // QPixmap a .png image
    QMap<QString, QPixmap*> allImages;


    // The instance function returns a pointer to a static variable and thus is declared static.
    static singletonRender* Instance();

    //will load all images in ../Data/Images/ into the allImages Map
    bool loadImages();

    /*
     * Renders the whole mesh
     */
    void renderMesh(Mesh *workMesh, QWidget* parent);

    /*
     * Renders a node.
     *
     */
    void renderNode(Node* nodeToRender, QWidget* parent, int nodeID);

    /*
     * Render one Node Type in the Catalog
     *
     */
    void renderNodeType(Node* nodeToRender,QWidget* parent, int position);

    /*
     * This will Render all Nodes in the Type Catalog.
     */
    void renderCatalogContent(QVector<Node> NodeVektor, QWidget *CatalogParent);


private:
    // Private so that it can  not be called
    // Only the class function Instance can call the constructor. Public access to the constructor is denied.
    singletonRender();

    //The constructor, copy constructor and assignment operator are all private to ensure that the
    //programmer using the singleton class can only create a single instance of the class using only the Instance() function.

    // copy constructor is private
    singletonRender(singletonRender const&){};

     // assignment operator is private
    singletonRender& operator=(singletonRender const&){};

    // pointer to this instance
    static singletonRender* m_pInstance;
};

#endif // SINGLETONRENDER_H













