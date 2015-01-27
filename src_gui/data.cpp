#include "data.h"
#include "singletonrender.h"
#include "ui_mainwindow.h"
#include "settingshandler.h"
#include "nodecatalog.h"
#include "nodefactory.h"
#include "anisecommunicator.h"
#include "QFileDialog"

// Global static pointer used to ensure a single instance of the class.
Data *Data::data = NULL;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/

Data *Data::instance() {
    if (!data)  // Only allow one instance of class to be generated.
        data = new Data;

    return data;
}

Data::Data(QObject *parent) : QObject(parent) {
    mesh = new Mesh();
    nodeCatalog = NodeCatalog::instance();
}

void  Data::initialize(MainWindow *mainWindow){

    // create the Node catalog
    NodeCatalog::instance();
    // create the render object
    SingletonRender::instance();
    //create NodeFactory
    NodeFactory::instance();

    // initalize stored Settings
    SettingsHandler::setSettingsPath(QApplication::applicationDirPath() +
                                     "/settings.ini");
    // qDebug() << QApplication::applicationDirPath() + "/settings.ini";

    // Check if Framework path is set

    if (SettingsHandler::contains("frameworkpath"))
        AniseCommunicator::setFrameworkPath(
                    SettingsHandler::loadSetting("frameworkpath"));
    else {
        /*
     * TODO different outcome of buttons
     * right now whatever you click will result in the same outcome
     */
        QMessageBox::information(
                    0, QString("Please, set your framework path"),
                    QString("You haven't set your framework path yet.\nChoose first!"),
                    "Choose", "Not yet");

        QString fileName =
                QFileDialog::getOpenFileName(mainWindow, "Set your framework path", "", "");

        SettingsHandler::storeSetting("frameworkpath", fileName);
    }

    // initialize settings from .ini file
    SettingsHandler::initializeSettings();
/*
    // create 20 test nodes
    for (int i = 0; i < 20; ++i) {
        Node *tempTestNode = NodeFactory::createTestNode();
        NodeCatalog::instance()->insert(*tempTestNode);
    }
*/


    // START LOADING NODE TYPES

    // load all available NodeTypes
    QString out = AniseCommunicator::getAllNodeTypes();
    qDebug() << "outputANISE: " << out;
    JsonFileHandler::parseNodeTypesFromAnise(out);

    // render the node catalog filled with test nodes
    SingletonRender::instance()->renderCatalogContent(NodeCatalog::instance()->Content.values().toVector());

}

void Data::addNodeToMesh(Node *newNode) {
    this->mesh->addNode(newNode);
    SingletonRender::instance()->renderMesh(this->mesh);
}

void Data::moveNodeInMesh(QPoint *Position, int numberOfNode) {
    this->mesh->getNodeByID(numberOfNode)
            ->setPosition(Position->x(), Position->y());
    SingletonRender::instance()->renderMesh(this->mesh);
}


Mesh *Data::getMesh(){

    return this->mesh;
}


void Data::newMeshProject(){

    //create new Mesh Object
    mesh = new Mesh();

    //clearMeshField
    SingletonRender::instance()->clearMeshField();

    //renderNewMesh
    SingletonRender::instance()->renderMesh(mesh);
}

bool Data::deleteItem(){

    bool deleted = this->mesh->deleteItem();
    //qDebug() << "Node Got Deleted: " << deleted;
    //qDebug() << "NodeMap: " << this->getMesh()->nodesInMash;
    //qDebug() << "Drawings: " << SingletonRender::instance()->getChildrenIDs()->toList();
    return deleted;

}
