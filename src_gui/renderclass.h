#ifndef RENDERCLASS_H
#define RENDERCLASS_H

/*
 * @author: Artur & Tobi
 * @date: 13.01.2015
 *
 *
 */

#include <QLabel>
#include <QMap>
#include <QVector>
#include <QImage>
#include <QWidget>
#include "node.h"
#include "mesh.h"

class RenderClass
{
public:

    RenderClass();

    static bool loadImages();


    // Catalog is a Map containing all *png files in teh image folder
    // Catalog["nameOfFile"] returns the picture
    static QMap<QString, QPixmap*> Catalog;

    static QWidget buildWidget(Node* node);

    /*
     * Renders the whole mesh
     */
    static void renderMesh(Mesh *workMesh, QWidget* parent);

    /*
     * Renders a node.
     *
     */
    static void renderNode(Node* nodeToRender,QWidget* parent);

    /*
     * Render one Node Type in the Catalog
     *
     */
    static void renderNodeType(Node* nodeToRender,QWidget* parent, int position);


    /*
     * This will Render all Nodes in the Type Catalog.
     */
    static void renderCatalogContent(QVector<Node> NodeVektor, QWidget *CatalogParent);

};

#endif // RENDERCLASS_H
