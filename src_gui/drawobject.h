#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>


class DrawObject
{
public:
    DrawObject(QWidget *parent );
    void show();
    QWidget *widget;
    QLabel *labelOfThis;

};

#endif // DRAWOBJECT_H
