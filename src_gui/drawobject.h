#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QLabel>



class DrawObject : public QLabel {

  //  Q_OBJECT

public:
    int nodeID;
    DrawObject(int nodeID, QWidget *parent);






};

#endif  // DRAWOBJECT_H
