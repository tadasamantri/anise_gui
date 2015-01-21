#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPainter>


class DrawObject : public QLabel {
public:
    int nodeID;
    DrawObject(int nodeID, QWidget *parent);
};


#endif  // DRAWOBJECT_H
