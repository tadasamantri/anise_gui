#include "drawobject.h"
#include <QShortcut>
#include <QSize>
#include <QDebug>
#include "gatebutton.h"

DrawObject::DrawObject(int id, QPoint position, int width, int height,
                       QWidget *parent = 0) {
    this->ID = id;
    this->setParent(parent);

    // We say the constructor which position he has
    this->setGeometry(position.x(), position.y(), width, height);

    //initialize mainMask with complete Transparency
    mainMask = QBitmap(this->size());
    mainMask.fill(Qt::color0);

    //initialize the over all picture
    this->overAllPicture = QPixmap (this->size());
    overAllPicture.fill(QColor(0,0,0,0)); //make it transparent

    QPixmap *background = new QPixmap(width, height);
    background->fill(Qt::red);
    this->addPicture(background, QPoint(0,0));
    mainMask.fill(Qt::color0);
    this->setMask(mainMask);

    mainMaskAsImage = mainMask.toImage();

    //connects
     connect(this, SIGNAL(released(int,QString, QPoint)), this->parent(), SLOT(handleGateClick(int, QString, QPoint)));
}

/**
 * Has two parameter for the widget.
 * We create label object.
 * we give the label the pixmap and the geometry.
 * Then we show it.
 *
 * @brief TestDrawObject::addPicture
 * @param pic       picture which you want to add
 * @param position  position relativ to top left corner
 */
void DrawObject::addPicture(QPixmap *pic, QPoint position) {

    //safe dimensions
    int height = pic->height();
    int width = pic->width();

    // Instanstiate the drawing to be shown
    QLabel *label = new QLabel(this);
    label->setGeometry(position.x(), position.y(), width, height);
    label->setPixmap(*pic);
    this->labelvector << label;
    // Now set the masks to the widgets
    label->setMask(pic->mask());

    //modifies the mainMask inserting the mask of the pic
    this->modifyMask(pic, position);

    // update the over all picture
    this->updateOverAllPicture(pic, position);



}

void DrawObject::addPicture(QPixmap *pic, QPoint position, QString typeName){

    QPixmap newPic = pic->copy(pic->rect());

    // tell the painter to draw on the QImage
    QPainter* painter = new QPainter(&newPic);
    painter->setPen(Qt::blue);
    painter->setFont(QFont("Arial", 8));
    // Write Typename onto picture
    painter->drawText(newPic.rect(), Qt::AlignLeading, typeName);


    //actually call addPicture with modified picture
    this->addPicture(&newPic, position);
    delete painter;
}

void DrawObject::addGateButton(QPixmap *pic, QPoint position, QString gateName) {

    //safe dimensions
    int height = pic->height();
    int width = pic->width();

    // Instanstiate the drawing to be shown
    GateButton *button = new GateButton(gateName, ID, this);
    button->setGeometry(position.x(), position.y(), width, height);
    button->setIcon(*pic);
    this->buttonvector << button;http://www.teamviewer.com/de/download/linux.aspx
    // Now set the masks to the widgets
    button->setMask(pic->mask());

    //modifies the mainMask inserting the mask of the pic
    this->modifyMask(pic, position);

    // update the over all picture
    this->updateOverAllPicture(pic, position);

    // Connect button signal to appropriate slot
    connect(button, SIGNAL(released(QString, QPoint)), this, SLOT(releasedOnGate(QString, QPoint)));





}



void DrawObject::modifyMask(QPixmap *pic, QPoint position){
    //safe dimensions
    int height = pic->height();
    int width = pic->width();

    // Instanstiate the non-transparent mask with size of pic
    QBitmap picMask = pic->mask();  // This is used to get the mask automatically
    // from the picture but doesnt work all the
    // time

    QBitmap highlightPicMask = QBitmap(width + 6, height + 6);

    
    // If there is no nontransparency, create nontransparentmask manually (no
    // other option so far)
    if (picMask.size().isEmpty()) {
        picMask = QBitmap(width, height);
        picMask.fill(Qt::color1);
    }
    
    

    // Include the picture (non-transparent) to the main Mask to get the area of
    // the picture to be visible
    QPainter painter(&mainMask);
    painter.setBrush(Qt::color1);
    painter.drawPixmap(position.x(), position.y(), width, height, picMask);


    //update changes to QImage
    mainMaskAsImage = mainMask.toImage().convertToFormat(QImage::Format_Mono);

    //Now set the mask of the widget
    this->setMask(mainMask);


}


void DrawObject::updateOverAllPicture(QPixmap *newPicture, QPoint position ){

    //save the dimension of the new picture
    int height = newPicture->height();
    int width = newPicture->width();

    // create a painter
    QPainter painter(&(this->overAllPicture));
    painter.setBrush(Qt::black);
    painter.drawPixmap(position.x(), position.y(), width, height, *newPicture);

    //highlightMask();
}

QPixmap DrawObject::getPicture() { return this->overAllPicture; }


void DrawObject::releasedOnGate(QString gateName, QPoint position){



    emit released(this->ID, gateName, this->pos() + position);
}

void DrawObject::highlightMask(){

   int width = this->width();
   int height = this->height();

   // this highlights left and right side
   for(int y = 0; y < height; y++){

       for(int x = 3; x < width - 3; x++){

           //left side highlight
           if(getPixel(mainMaskAsImage, x, y) && !getPixel(mainMaskAsImage, x-1, y)){
               //setPixel(mainMaskAsImage, x-5, y, 1);
               //setPixel(mainMaskAsImage, x-4, y, 1);
               setPixel(mainMaskAsImage, x-3, y, 1);
               setPixel(mainMaskAsImage, x-2, y, 1);
               setPixel(mainMaskAsImage, x-1, y, 1);
           }

           // the right side of drawobject to highlighting
           if(getPixel(mainMaskAsImage, x, y) && !getPixel(mainMaskAsImage, x+1, y)){
               setPixel(mainMaskAsImage, x+1, y, 1);
               setPixel(mainMaskAsImage, x+2, y, 1);
               setPixel(mainMaskAsImage, x+3, y, 1);
               //setPixel(mainMaskAsImage, x+4, y, 1);
               //setPixel(mainMaskAsImage, x+5, y, 1);
               break;
            }


       }

   }

   // this highlight upside and downside
   for(int x = 0; x < width; x++){

       for(int y = 3; y < height - 3; y++){

           //highlights the upside of drawobject
           if(getPixel(mainMaskAsImage, x, y) && !getPixel(mainMaskAsImage, x, y-1)){
               //setPixel(mainMaskAsImage, x, y-5, 1);
               //setPixel(mainMaskAsImage, x, y-4, 1);
               setPixel(mainMaskAsImage, x, y-3, 1);
               setPixel(mainMaskAsImage, x, y-2, 1);
               setPixel(mainMaskAsImage, x, y-1, 1);
           }

           //highlights the downside of drawobject
           if(getPixel(mainMaskAsImage, x, y) && !getPixel(mainMaskAsImage, x, y+1)){
               setPixel(mainMaskAsImage, x, y+1, 1);
               setPixel(mainMaskAsImage, x, y+2, 1);
               setPixel(mainMaskAsImage, x, y+3, 1);
               //setPixel(mainMaskAsImage, x, y+4, 1);
               //setPixel(mainMaskAsImage, x, y+5, 1);
               break;
            }


       }

   }

   mainMask = QBitmap::fromImage(mainMaskAsImage);

   this->setMask(mainMask);
   // labelvector.first()->setMask(mainMask);



}

int DrawObject::getPixel(const QImage& img, const int x, const int y) const
{


    const uchar mask = 0x80 >> (x % 8);
    int pixel = img.scanLine(y)[x / 8] & mask ? 1 : 0;
    //qDebug() << pixel << ",";
    return pixel;

}

void DrawObject::printMask(){

    int width =  this->width();
    int height = this->height();
    int pixel;
    QString row ="";
    QDebug deb = qDebug();

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){

            deb.nospace() << getPixel(mainMaskAsImage, j, i);


        }
       // deb << row;
        row="";
        deb = qDebug();


    }

}




void DrawObject::setPixel(QImage& img, const int x, const int y, const int pixel)
{
    const uchar mask = 0x80 >> (x % 8);
    if (pixel)
        img.scanLine(y)[x / 8] |= mask;
    else
        img.scanLine(y)[x / 8] &= ~mask;
}
