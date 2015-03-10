#include "drawobject.h"
#include <QShortcut>
#include <QSize>
#include <QDebug>
#include <QStyleOption>
#include <QListWidget>
#include <QObject>
#include <QRegion>
#include "gatebutton.h"
#include "mesheditorwidget.h"
#include "data.h"

DrawObject::DrawObject(int id, QPoint position, int width, int height,
                       QWidget *parent, int highlightOffset) {
    this->ID = id;
    this->setParent(parent);
    this->highlightWidth = highlightOffset;
    this->nameLabel = 0;
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

void DrawObject::addPicture(QPixmap *pic, QPoint position, QString nodeName) {
    QPixmap newPic = pic->copy(pic->rect());

    // tell the painter to draw on the QImage
    QPainter *painter = new QPainter(&newPic);
    painter->setPen(Qt::blue);
    painter->setFont(QFont("Arial", 8));
    // Write Typename onto picture
    //painter->drawText(newPic.rect(), Qt::AlignLeading, nodeName);




    // actually call addPicture with modified picture
    this->addPicture(&newPic, position);

    delete painter;
}


void DrawObject::nodeNameChanged(QListWidgetItem * itemChanged){

    itemChanged->setSelected(false);
    Node *node = Data::instance()->getMesh()->getNodeByID(this->ID);

    if(node->getName() != itemChanged->text())
        node->setName(itemChanged->text());

      nameLabel->setStyleSheet("background: transparent;");

}


void DrawObject::setNodeName(QString nodeName){

/*
    if(!nameLabel)
        nameLabel = new QLabel(dynamic_cast<MeshEditorWidget *> (this->parent()));

    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setText(nodeName);
    nameLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    nameLabel->adjustSize();

    int posX = this->pos().x()+this->width()/2-nameLabel->width()/2;
    int posY = this->pos().y()+this->height();
    nameLabel->setGeometry(posX, posY, nameLabel->width(), nameLabel->height());
    nameLabel->show();*/


     //----------Masterlösung mit editierbarem Fenster, aber noch verbuggt
    if(!nameLabel){
        nameLabel = new QListWidget(dynamic_cast<MeshEditorWidget *> (this->parent()));
    QListWidgetItem *item = new QListWidgetItem(nameLabel);


    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    nameLabel->setFrameStyle(QFrame::NoFrame);
    //,QListWidget::item:selected{background: transparent}, QListWidget::item:hover, QListWidget::item:disabled:hover,QListWidget::item:hover:!active;} QMenu::item:selected {background-color: #432fecd3},
    nameLabel->setStyleSheet("background: transparent;");
    nameLabel->setStyleSheet("background: transparent;");

    item->setText(nodeName);
    nameLabel->insertItem(0, item);

    int posX = this->pos().x()+this->width()/2-nameLabel->width()/2;
    int posY = this->pos().y()+this->height();
    nameLabel->setGeometry(posX, posY, this->width()*2, 20);
        connect(nameLabel, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(nodeNameChanged(QListWidgetItem*)));
        connect(nameLabel, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(restrictOneClickOneItem(QListWidgetItem*)));
        connect(nameLabel, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(deleteItemText(QListWidgetItem*)));
    }


    nameLabel->itemAt(1,1)->setText(nodeName);
    nameLabel->show();



    //this->labelvector.append(nameOfNode);
   // modifyMask(new QPixmap(this->width(),20), QPoint(0, this->height()-20), false);

}

void DrawObject::restrictOneClickOneItem(QListWidgetItem *itemClicked){

    itemClicked->setSelected(false);
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

void DrawObject::modifyMask(QPixmap *pic, QPoint position, bool updateMask) {
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

    if(updateMask)
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


            if(gate->getDirection() && gate->getGateType() == gateType)
                gate->setHighlightMode(true);
            else
                gate->setHighlightMode(false);
     }
}

void DrawObject::dehighlightGates(){

    foreach(GateButton *gate, gateVector){

        //if(gate->isInput())
            gate->resetPicture();
    }

}



void DrawObject::move(int x, int y){

    QWidget::move(x, y);

    int nodeNameX = this->pos().x()+this->width()/2-nameLabel->width()/2;
    int nodeNameY = this->pos().y()+this->height();

    if(nameLabel)
        nameLabel->move(nodeNameX,nodeNameY);

}

void DrawObject::hide(){

    QWidget::hide();
    nameLabel->hide();
}

void DrawObject::show(){

    QWidget::show();
    nameLabel->show();
}

void DrawObject::deleteLater(){

    QObject::deleteLater();
    nameLabel->deleteLater();
}

void DrawObject::deleteItemText(QListWidgetItem * item){

    //item->setBackgroundColor(Qt::white);
    nameLabel->setStyleSheet("QListWidget::item:selected{background: transparent}");
}
