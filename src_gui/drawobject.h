#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QVector>
#include <QBitmap>
#include <QPushButton>

class DrawObject : public QWidget {

   Q_OBJECT

public:
    int ID;
    QVector<QLabel*> labelvector;

    DrawObject(int id, QPoint position, int width , int height, QWidget *parent = 0, int highlightWidth = 3);

    QVector<QPushButton*> buttonvector;



    QBitmap mainMaskUnhighlighted;
    QBitmap mainMaskHighlighted;
    QImage mainMaskAsImage;
    QPainter painter;
    QPixmap overAllPicture;

    void addPicture (QPixmap *pic, QPoint position);
    void addPicture(QPixmap *pic, QPoint position, QString typeName);
    void addGateButton(QPixmap *pic, QPoint position, QString gateName);

    //Modify the mask of the drawobject inserting the mask of the pixmap at given position
    void modifyMask(QPixmap *pic, QPoint position);

    QPixmap getPicture ();


    void dehighlight();
    void highlight();
    void printMask();

    void paintEvent(QPaintEvent *);
    int getHighlightWidth() const;


signals:

    void released(int nodeID, QString gateName, QPoint position);

public slots:

    void releasedOnGate(QString gateName, QPoint position);

private:

    int highlightWidth;

void updateOverAllPicture(QPixmap *newPicture, QPoint position);
int getPixel(const int x, const int y) const;
void setPixel(const int x, const int y, const int pixel);
void highlightMask();

};


#endif // DRAWOBJECT_H
