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
    int ID;
    QVector<QLabel*> labelvector;
    DrawObject(int id, QPoint position, int width , int height, QWidget *parent);

    QBitmap  mainMask;
    QPainter painter;
    QPixmap overAllPicture;

    void addPicture (QPixmap *pic, QPoint position);
    void addPicture(QPixmap *pic, QPoint position, QString typeName);


    QPixmap getPicture ();

private:

void updateOverAllPicture(QPixmap *newPicture, QPoint position);

};


#endif // DRAWOBJECT_H
