#include "renderclass.h"
#include <QBitmap>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QtWidgets>
#include <QMap>

RenderClass::RenderClass()
{
    //load all images
    if (loadImages() == true) {
        qDebug() << "images loaded successfully";


//        //create Node images
//        for (int i = 0; i < Catalog.size(); ++i) {
//            QLabel *temp = new QLabel();
//            temp->setPixmap(*Catalog.at(i));
//            temp->setParent(parent);
//            //temp.setMask(Catalog.at(i)->mask());
//            temp->setGeometry(0,i * (Catalog.at(i)->height()),Catalog.at(i)->width() ,Catalog.at(i)->height());
//            Nodes<< temp ;
//        }
    }else{
        qDebug() << "images loading failed";
    }
}



//loads all images in the image folder.
//saves them in a vector
bool RenderClass::loadImages(){
    bool result = true;
    QStringList listOfFiles;

    //absoluter pfad muss noch geändert werden! TODO
    QDir directory("/media/Work/BaPra/GuiRepo/anise_gui/Data/Images/");

    if (directory.exists() == true) {
        //qDebug() << "directory.exists() == true";
        listOfFiles = directory.entryList();
    }else{
        qDebug() << "directory.exists() == false";
        result = false;
    }


    listOfFiles = listOfFiles.filter(".png");


    /*
    for (int i = 0; i < listOfFiles.size(); ++i) {
        qDebug() << "file found: " << listOfFiles.at(i);
    }
*/

    for (int i = 0; i < listOfFiles.size(); ++i) {
        if (result == true) {


            QPixmap *temp = new QPixmap();
            result = temp->load(directory.absolutePath().append("/"+listOfFiles.at(i)));

            if (result == false) {
                qDebug() << "loaded image: " << directory.absolutePath().append(listOfFiles.at(i)) << " " << result;
            }

            Catalog.insert(listOfFiles.at(i), temp);

        }
    }



    return result;
}


void RenderClass::renderNode(Node* nodeToRender,QWidget* parent){


    QWidget *NodeWidget = new QWidget(parent);

    //Zeichne den hintergrund:
    QLabel *BackgroundLabel = new QLabel();

    //Momentan hardcoded bild für Background. soll per id oder string ansprechbar sein!
    BackgroundLabel->setPixmap(*Catalog["background.png"]);
    BackgroundLabel->setParent(NodeWidget);

    NodeWidget->setGeometry(int(nodeToRender->position_x),int(nodeToRender->position_y), BackgroundLabel->width(), BackgroundLabel->height());
    NodeWidget->show();
    //Nodes<< BackgroundLabel ;

}


