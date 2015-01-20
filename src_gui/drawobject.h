#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>

class DrawObject : public QLabel {

public:
  int numberOfNode;
  DrawObject(int numberOfNode, QWidget *parent);
};

#endif // DRAWOBJECT_H
