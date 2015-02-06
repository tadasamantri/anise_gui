#include "drawobject.h"
#include <QShortcut>
#include <QSize>

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


    //connects
     connect(this, SIGNAL(released(int)), this->parent(), SLOT(handleGateClick(int)));
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
    this->labelvector.append(label);
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

}

void DrawObject::addButton(QPixmap *pic, QPoint position) {

    //safe dimensions
    int height = pic->height();
    int width = pic->width();

    // Instanstiate the drawing to be shown
    QPushButton *button = new QPushButton(this);
    button->setGeometry(position.x(), position.y(), width, height);
    button->setIcon(*pic);
    this->buttonvector.append(button);
    // Now set the masks to the widgets
    button->setMask(pic->mask());

    //modifies the mainMask inserting the mask of the pic
    this->modifyMask(pic, position);

    // update the over all picture
    this->updateOverAllPicture(pic, position);

    // Connect button signal to appropriate slot
    connect(button, SIGNAL(released()), this, SLOT(releaseWithParam()));





}



void DrawObject::modifyMask(QPixmap *pic, QPoint position){
    //safe dimensions
    int height = pic->height();
    int width = pic->width();

    // Instanstiate the non-transparent mask with size of pic
    QBitmap picMask = pic->mask();  // This is used to get the mask automatically
    // from the picture but doesnt work all the
    // time

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

}

QPixmap DrawObject::getPicture() { return this->overAllPicture; }


void DrawObject::releaseWithParam(){

    emit released(this->ID);
}
