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
    /**
      * Allows only one instance of class to be generated.
      */
    if (!data)
        data = new Data;

    return data;
}

Data::Data(QObject *parent) : QObject(parent) {
    mesh = new Mesh();
    nodeCatalog = NodeCatalog::instance();
}

void  Data::initialize(MainWindow *mainWindow){

    /**
      * Create the Nodecatalog
      */
    NodeCatalog::instance();
    /**
      * Create the render object
      */
    SingletonRender::instance();
    /**
      * Create the NodeFactory
      */
    this->nodeFactory = new NodeFactory();

    /**
      * Initialize stored settings
      */
    SettingsHandler::setSettingsPath(QApplication::applicationDirPath() +
                                     "/settings.ini");

    /**
      * Checks if Framework path is set
      */
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

    /**
      * Initialize settings from .ini file
      */
    SettingsHandler::initializeSettings();
/*
    //create 20 test nodes

    for (int i = 0; i < 20; ++i) {
        Node *tempTestNode = NodeFactory::createTestNode();
        NodeCatalog::instance()->insert(*tempTestNode);
    }
*/

    /**
      * Start loading node types
      * Load all available NodeTypes
      */
    QString out = AniseCommunicator::getAllNodeTypes();
    qDebug() << "outputANISE: " << out;
    JsonFileHandler::parseNodeTypesFromAnise(out);

    /**
      * Render the Nodecatalog filled with test nodes
      */
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

void Data::deleteItem(int nodeID){

    qDebug() << "HEY SOMEBODY GOT DELETED" << " nodeID: " << nodeID;

}


void Data::newMeshProject(){

    /**
      * Create new Mesh object
      */
    mesh = new Mesh();

    /**
      * Clears a Meshfield
      */
    SingletonRender::instance()->clearMeshField();

    /**
      * Render a NewMesh
      */
    SingletonRender::instance()->renderMesh(mesh);
}
