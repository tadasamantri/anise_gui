#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QVector>
#include <QBitmap>



class DrawObject : public QWidget {

  //  Q_OBJECT

public:
    int nodeID;
    QVector<QLabel*> labelvector;
    DrawObject(int nodeID, QPoint position, QWidget *parent);
    QBitmap  mask;
    QPainter painter;

void addPicture (QPixmap *pic, QPoint position, int width, int height);

void getPicture ();

void show();
};


#endif // DRAWOBJECT_H
