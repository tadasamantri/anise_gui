#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QVector>
#include <QBitmap>
#include <QPushButton>



class DrawObject : public QWidget {

  //  Q_OBJECT

public:
    int nodeID;
    QVector<QLabel*> labelvector;
    QVector<QPushButton*> buttonvector;
    DrawObject(int nodeID, QPoint position, int height, QWidget *parent);

    QBitmap  mainMask;
    QPainter painter;
    QPixmap overAllPicture;

    void addPicture (QPixmap *pic, QPoint position);
    void addPicture(QPixmap *pic, QPoint position, QString typeName);
    void addButton(QPixmap *pic, QPoint position);

    //Modify the mask of the drawobject inserting the mask of the pixmap at given position
    void modifyMask(QPixmap *pic, QPoint position);

    QPixmap getPicture ();

private:

void updateOverAllPicture(QPixmap *newPicture, QPoint position);

};


#endif // DRAWOBJECT_H
