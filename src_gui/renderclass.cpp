#include "renderclass.h"
#include <QBitmap>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QtWidgets>
#include <QMap>
#include "node.h"
#include <QVector>

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

    QDir directory;
    directory.cdUp();
    directory.cd("Data/Images/");

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

    BackgroundLabel->setPixmap(*Catalog["background.png"]);
    BackgroundLabel->setParent(NodeWidget);

    NodeWidget->setGeometry(int(nodeToRender->position_x),int(nodeToRender->position_y), BackgroundLabel->width(), BackgroundLabel->height());
    NodeWidget->show();
    //Nodes<< BackgroundLabel ;

}

void RenderClass::renderNodeType(Node* nodeToRender,QWidget* parent, int position){
    QWidget *NodeWidget = new QWidget(parent);

    //Zeichne den hintergrund:
    QLabel *BackgroundLabel = new QLabel();

    BackgroundLabel->setPixmap(*Catalog["background.png"]);
    BackgroundLabel->setParent(NodeWidget);


    //TO-DO should use layouts instead of hardcoded position!
    NodeWidget->setGeometry(5,position * 55, BackgroundLabel->width(), BackgroundLabel->height());
    NodeWidget->show();
    //Nodes<< BackgroundLabel ;


}

void RenderClass::renderCatalogContent(QVector<Node> NodeVektor, QWidget *CatalogParent){
    int position = 0;
    foreach (Node nodeTyp, NodeVektor) {
        renderNodeType(&nodeTyp, CatalogParent, position);
        position++;
    }
}

