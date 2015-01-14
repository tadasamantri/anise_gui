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

class RenderClass
{
public:
    QLabel testBild;
    RenderClass(QWidget* parent);

    bool loadImages();
    QLabel rose;
    QVector<QLabel*> Nodes;
    QVector<QPixmap*> Catalog; //All pictures that we load!

    void testBildRender();
};

#endif // RENDERCLASS_H
