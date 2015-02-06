#ifndef TESTDRAWOBJECT_H
#define TESTDRAWOBJECT_H

#include <QLabel>



class TestDrawObject : public QWidget {

  //  Q_OBJECT

public:
    int nodeID;
    TestDrawObject(int nodeID, QPoint position, QWidget *parent);

void addPicture (QPixmap *pic, QPoint position);

void getPicture ();





};


#endif // TESTDRAWOBJECT_H
