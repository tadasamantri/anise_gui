#include "data.h"
#include "renderclass.h"
#include "ui_mainwindow.h"

Data::Data(QObject *parent) :
    QObject(parent)
{

    mesh = new Mesh();
    nodeCatalog = new NodeCatalog();
}



void Data::setUi(Ui::MainWindow *ui){
    this->ui = ui;
}


void Data::addNodeToMesh(Node *newNode){
    this->mesh->addNode(newNode);
    RenderClass::renderMesh(this->mesh, this->ui->mesh_edt_area);
}
