#include "renderclass.h"
#include <QBitmap>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QtWidgets>
#include <QMap>
#include "node.h"
#include "dragwidget.h"
#include <QVector>
#include "drawobject.h"
#include "data.h"
#include "dataholder.h"

QMap<QString, QPixmap*> RenderClass::Catalog;

RenderClass::RenderClass() {
  // load all images
  if (loadImages() == true) {
    qDebug() << "images loaded successfully";

    //        //create Node images
    //        for (int i = 0; i < Catalog.size(); ++i) {
    //            QLabel *temp = new QLabel();
    //            temp->setPixmap(*Catalog.at(i));
    //            temp->setParent(parent);
    //            //temp.setMask(Catalog.at(i)->mask());
    //            temp->setGeometry(0,i *
    //            (Catalog.at(i)->height()),Catalog.at(i)->width()
    //            ,Catalog.at(i)->height());
    //            Nodes<< temp ;
    //        }
  } else {
    qDebug() << "images loading failed";
  }
}

// loads all images in the image folder.
// saves them in a vector
bool RenderClass::loadImages() {
  bool result = true;
  QStringList listOfFiles;

  QDir directory;
  directory.cdUp();
  directory.cd("Data/Images/");

  if (directory.exists() == true) {
    // qDebug() << "directory.exists() == true";
    listOfFiles = directory.entryList();
  } else {
    qDebug() << "directory.exists() == false";
    result = false;
  }

  listOfFiles = listOfFiles.filter(".png");

  // loads all images; if one coudlnt be loaded it will return false
  for (int i = 0; i < listOfFiles.size(); ++i) {
    if (result == true) {

      QPixmap *temp = new QPixmap();
      result =
          temp->load(directory.absolutePath().append("/" + listOfFiles.at(i)));

      if (result == false) {
        qDebug() << "loaded image: "
                 << directory.absolutePath().append(listOfFiles.at(i)) << " "
                 << result;
      }

      Catalog.insert(listOfFiles.at(i), temp);
    }
  }

  return result;
}

void RenderClass::renderMesh(Mesh *workMesh, QWidget *parent) {

  // kills all children
  foreach (QObject *child, parent->children()) { child->deleteLater(); }
  int i = 0;

  // calls render method for every node in the mesh
  foreach (Node *node, workMesh->nodes) {
    RenderClass::renderNode(node, parent, i);
    i++;
  }
}

void RenderClass::renderNode(Node *nodeToRender, QWidget *parent,
                             int numberOfNode) {

  // TO-DO code dublication in renderNode and renderNodeType!
  DrawObject *NodeDrawObject = new DrawObject(numberOfNode, parent);
  // qDebug() << NodeDrawObject->parent();

  // Zeichne den hintergrund:
  // NodeDrawObject->setParent(parent);
  NodeDrawObject->setPixmap(*Catalog["background.png"]);
  NodeDrawObject->setGeometry(0, 0, 50, 50);

  // TO-DO should use layouts instead of hardcoded position!
  NodeDrawObject->move(nodeToRender->position_x, nodeToRender->position_y);

  // NodeDrawObject->labelOfThis->setGeometry(x,y,height,width);
  NodeDrawObject->show();
}

void RenderClass::renderNodeType(Node *nodeToRender, QWidget *parent,
                                 int position) {

  // TO-DO code dublication in renderNode and renderNodeType!
  QLabel *NodeDrawObject = new QLabel(parent);
  // qDebug() << NodeDrawObject->parent();

  // Zeichne den hintergrund:
  // NodeDrawObject->setParent(parent);
  NodeDrawObject->setPixmap(*Catalog["background.png"]);
  NodeDrawObject->setGeometry(0, 0, 50, 50);

  // TO-DO should use layouts instead of hardcoded position!
  NodeDrawObject->move(5, 5 + position * 60);

  // NodeDrawObject->labelOfThis->setGeometry(x,y,height,width);
  NodeDrawObject->show();
}

void RenderClass::renderCatalogContent(QVector<Node> NodeVektor,
                                       QWidget *CatalogParent) {
  int position = 0;
  // TO-DO scroll weite sollte nicht hard coded sein
  CatalogParent->setMinimumHeight(NodeVektor.size() * 60 + 10);
  foreach (Node nodeTyp, NodeVektor) {
    renderNodeType(&nodeTyp, CatalogParent, position);
    position++;
  }
}
