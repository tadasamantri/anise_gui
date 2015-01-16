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

class RenderClass
{
public:

    RenderClass();

    bool loadImages();


    // Catalog is a Map containing all *png files in teh image folder
    // Catalog["nameOfFile"] returns the picture
    QMap<QString, QPixmap*> Catalog;

    QWidget buildWidget(Node* node);

    void testBildRender();

    /*
     * Renders a node.
     *
     */
    void renderNode(Node* nodeToRender,QWidget* parent);

    /*
     * Render one Node Type in the Catalog
     *
     */
    void renderNodeType(Node* nodeToRender,QWidget* parent, int position);


    /*
     * This will Render all Nodes in the Type Catalog.
     */
    void renderCatalogContent(QVector<Node> NodeVektor, QWidget *CatalogParent);

};

#endif // RENDERCLASS_H
