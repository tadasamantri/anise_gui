#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QLabel>



class DrawObject : public QWidget {

  //  Q_OBJECT

public:
    int nodeID;
    DrawObject(int nodeID, QPoint position, QWidget *parent);

void addPicture (QPixmap *pic, QPoint position);

void getPicture ();





};


#endif // DRAWOBJECT_H
