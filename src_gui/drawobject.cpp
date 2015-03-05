#include "drawobject.h"
#include <QShortcut>
#include <QSize>
#include <QDebug>
#include <QStyleOption>
#include "gatebutton.h"

DrawObject::DrawObject(int id, QPoint position, int width, int height,
                       QWidget *parent, int highlightOffset) {
    this->ID = id;
    this->setParent(parent);
    this->highlightWidth = highlightOffset;
    // We say the constructor which position he has
    this->setGeometry(position.x(), position.y(), width + 2 * highlightOffset,
                      height + 2 * highlightOffset);

    // initialize mainMask with complete Transparency
    mainMaskUnhighlighted = QBitmap(this->size());
    mainMaskUnhighlighted.fill(Qt::color0);

    // initialize the over all picture
    this->overAllPicture = QPixmap(this->size());

    // make it transparent
    overAllPicture.fill(QColor(0, 0, 0, 0));
    // set backgroundcolor
    this->setStyleSheet("background-color:yellow;");

    mainMaskUnhighlighted.fill(Qt::color0);
    this->setMask(mainMaskUnhighlighted);

    mainMaskAsImage = mainMaskUnhighlighted.toImage();
    //connect release event
    connect(this, SIGNAL(released(int, QString, QPoint)), this->parent(),
            SLOT(handleGateClick(int, QString, QPoint)));
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
    // safe dimensions
    int height = pic->height();
    int width = pic->width();

    // take respect to highlightWidth
    position += QPoint(highlightWidth, highlightWidth);

    // Instanstiate the drawing to be shown
    QLabel *label = new QLabel(this);
    label->setGeometry(position.x(), position.y(), width, height);
    label->setPixmap(*pic);
    this->labelvector << label;
    // Now set the masks to the widgets
    label->setMask(pic->mask());

    // modifies the mainMask inserting the mask of the pic
    this->modifyMask(pic, position);

    // update the over all picture
    this->updateOverAllPicture(pic, position);
}

void DrawObject::addPicture(QPixmap *pic, QPoint position, QString typeName) {
    QPixmap newPic = pic->copy(pic->rect());

    // tell the painter to draw on the QImage
    QPainter *painter = new QPainter(&newPic);
    painter->setPen(Qt::blue);
    painter->setFont(QFont("Arial", 8));
    // Write Typename onto picture
    painter->drawText(newPic.rect(), Qt::AlignLeading, typeName);

    // actually call addPicture with modified picture
    this->addPicture(&newPic, position);
    delete painter;
}

void DrawObject::addGateButton(QPixmap *pic, QPoint position,
                               QString gateName, QString gateType, bool direction) {
    // safe dimensions
    int height = pic->height();
    int width = pic->width();

    // take respect to highlightWidth
    position += QPoint(highlightWidth, highlightWidth);

    // Instanstiate the drawing to be shown
    GateButton *button = new GateButton(gateName, gateType, ID, direction, this);
    button->setGeometry(position.x(), position.y(), width, height);
    button->setIcon(*pic);
    this->gateVector << button;
    // Now set the masks to the widgets
    button->setMask(pic->mask());

    // modifies the mainMask inserting the mask of the pic
    this->modifyMask(pic, position);

    // update the over all picture
    this->updateOverAllPicture(pic, position);

    // Connect button signal to appropriate slot
    connect(button, SIGNAL(released(QString, QPoint)), this,
            SLOT(releasedOnGate(QString, QPoint)));
}

QPoint DrawObject::getGatePosition(QString gateName)
{

    QPoint gatePosition(-1,-1);

    foreach(GateButton * gate, gateVector){

        if(gate->getGateName() == gateName){
            gatePosition = gate->pos();
            break;
        }
    }

    return gatePosition;
}

void DrawObject::modifyMask(QPixmap *pic, QPoint position) {
    // safe dimensions
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
    QPainter painter(&mainMaskUnhighlighted);
    painter.setBrush(Qt::color1);
    painter.drawPixmap(position.x(), position.y(), width, height, picMask);

    // update changes to QImage
    mainMaskAsImage =
            mainMaskUnhighlighted.toImage().convertToFormat(QImage::Format_Mono);
    highlightMask();

    // Now set the mask of the widget
    this->setMask(mainMaskUnhighlighted);
}

void DrawObject::updateOverAllPicture(QPixmap *newPicture, QPoint position) {
    // save the dimension of the new picture
    int height = newPicture->height();
    int width = newPicture->width();

    // create a painter
    QPainter painter(&(this->overAllPicture));
    painter.setBrush(Qt::black);
    painter.drawPixmap(position.x(), position.y(), width, height, *newPicture);
}

QPixmap DrawObject::getPicture() { return this->overAllPicture; }

void DrawObject::dehighlight() { this->setMask(mainMaskUnhighlighted); }

void DrawObject::highlight() { this->setMask(mainMaskHighlighted); }

void DrawObject::releasedOnGate(QString gateName, QPoint position) {
    emit released(this->ID, gateName, this->pos() + position);
}
int DrawObject::getHighlightWidth() const { return highlightWidth; }

void DrawObject::highlightMask() {
    int width = this->width();
    int height = this->height();

    // this highlights left and right side
    for (int y = 0; y < height; y++) {
        for (int x = highlightWidth; x < width - highlightWidth; x++) {
            // left side highlight
            if (getPixel(x, y) && !getPixel(x - 1, y)) {
                for (int highlightOffset = 0; highlightOffset < highlightWidth;
                     highlightOffset++)
                    setPixel(x - highlightOffset - 1, y, 1);
            }

            // the right side of drawobject to highlighting
            if (getPixel(x, y) && !getPixel(x + 1, y)) {
                for (int highlightOffset = 0; highlightOffset < highlightWidth;
                     highlightOffset++)
                    setPixel(x + highlightOffset + 1, y, 1);
                x += 4;
            }
        }
    }

    // this highlight upside and downside
    for (int x = 0; x < width; x++) {
        for (int y = highlightWidth; y < height - highlightWidth; y++) {
            // highlights the upside of drawobject
            if (getPixel(x, y) && !getPixel(x, y - 1)) {
                for (int highlightOffset = 0; highlightOffset < highlightWidth;
                     highlightOffset++)
                    setPixel(x, y - highlightOffset - 1, 1);
            }

            // highlights the downside of drawobject
            if (getPixel(x, y) && !getPixel(x, y + 1)) {
                for (int highlightOffset = 0; highlightOffset < highlightWidth;
                     highlightOffset++)
                    setPixel(x, y + highlightOffset + 1, 1);

                y += 4;
            }
        }
    }

    mainMaskHighlighted = QBitmap::fromImage(mainMaskAsImage);
}

int DrawObject::getPixel(const int x, const int y) const {
    const uchar mask = 0x80 >> (x % 8);
    int pixel = mainMaskAsImage.scanLine(y)[x / 8] & mask ? 1 : 0;

    return pixel;
}

void DrawObject::printMask() {
    int width = this->width();
    int height = this->height();
    QDebug deb = qDebug();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) deb.nospace() << getPixel(j, i);

        deb = qDebug();
    }
}

void DrawObject::setPixel(const int x, const int y, const int pixel) {
    const uchar mask = 0x80 >> (x % 8);
    if (pixel)
        mainMaskAsImage.scanLine(y)[x / 8] |= mask;
    else
        mainMaskAsImage.scanLine(y)[x / 8] &= ~mask;
}

void DrawObject::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DrawObject::highlightGates(QString gateType){

    foreach(GateButton *gate, gateVector){
        //if(gate->isInput()){

            if(gate->getDirection() && gate->getGateType() == gateType)
                gate->setHighlightMode(true);
            else
                gate->setHighlightMode(false);
        //}
    }
}

void DrawObject::dehighlightGates(){

    foreach(GateButton *gate, gateVector){

        //if(gate->isInput())
            gate->resetPicture();
    }

}
