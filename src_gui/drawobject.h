#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPainter>



class DrawObject : public QLabel {

  //  Q_OBJECT

public:
    int nodeID;
    DrawObject(int nodeID, QWidget *parent);

//public slots:
   // void deleteItem();
};

#endif  // DRAWOBJECT_H
