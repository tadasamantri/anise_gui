#include "data.h"
#include "settingshandler.h"
#include "nodefactory.h"
#include "anisecommunicator.h"
#include <QFileDialog>
#include <QMessageBox>

/**
 * @brief Data::data pointer to instance of this singleton class
 */
Data *Data::data = 0;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.
*/
Data *Data::instance() {
    /**
  * Allows only one instance of class to be generated.
  */
    if (!data) data = new Data;
    return data;
}

Data::Data(QObject *parent) : QObject(parent) {
    mesh = new Mesh();
    nodeCatalog = 0;
    nodeFactory = 0;
    saveFile = "";
    framework = new AniseCommunicator();
    runMode = false;

    SingletonRender *renderer = SingletonRender::instance();

    connect(this, SIGNAL(runModeChanged()), renderer, SLOT(changeProgressView()));
}
bool Data::isRunning() const
{
    return runMode;
}

void Data::testChangeRun(){

    runMode = !runMode;

    emit runModeChanged();

}

QString Data::getSaveFile() const
{
    return saveFile;
}

void Data::setSaveFile(const QString &value)
{
    saveFile = value;
}

MainWindow *Data::getMainWindow() const { return mainWindow; }

void Data::setMainWindow(MainWindow *value) { mainWindow = value; }

/**
 * @brief Data::initialize initilize all important containers.
 * @param mainWindow
 */
void Data::initialize(MainWindow *mainWindow) {
    this->mainWindow = mainWindow;
    //connect start and stop signals
    connect(mainWindow->ui->start_button, SIGNAL(clicked()),this,SLOT(startSimulation()));
    connect(mainWindow->ui->stop_button, SIGNAL(clicked()),this,SLOT(stopSimulation()));
    /**
  * Create the Nodecatalog
  */
    if(nodeCatalog)
        delete nodeCatalog;
    nodeCatalog = new NodeCatalog();
    /**
  * Create the render object
  */
    SingletonRender::instance();
    /**
  * Create the NodeFactory
  */
    if(nodeFactory)
        delete nodeFactory;
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

        QString fileName = QFileDialog::getOpenFileName(
                    mainWindow, "Set your framework path", "", "");

        SettingsHandler::storeSetting("frameworkpath", fileName);
    }

    /**
  * Initialize settings from .ini file
  */
    SettingsHandler::initializeSettings();

    /**
  * Start loading node types
  * Load all available NodeTypes
  */
    QString out = AniseCommunicator::getAllNodeTypes();
    JsonFileHandler::parseNodeTypesFromAnise(out);

    /**
  * Render the Nodecatalog filled with test nodes
  */
    SingletonRender::instance()->renderCatalogContent(
                Data::instance()->getNodeCatalog()->getContentVector());
}
/**
 * @brief Data::addNodeToMesh inserts given node into the mesh and renders it afterwards
 * @param newNode
 * @return ID of inserted Node
 */
int Data::addNodeToMesh(Node *newNode) {
    int id = this->mesh->addNode(newNode);
    // A new created Node is always focussed in the beginning

    if(id >= 0){
        SingletonRender::instance()->renderMesh();
        changed = true;
    }
    mesh->setFocusMeshObject(id);

    return id;
}

int Data::addNode(Node *node)
{
    return addNodeToMesh(node);
}

/**
 * @brief Data::addConnectionToMesh inserts new Connection to mesh
 * @param newConnection
 * @return
 * @see Data::addNodeToMesh
 */
int Data::addConnectionToMesh(Connection *newConnection) {
    int id = this->mesh->addConnection(newConnection);
    if(id >= 0){
        SingletonRender::instance()->renderMesh();
        changed = true;
    }
    return id;
}

int Data::addConnection(Connection *newConnection)
{
    return addConnectionToMesh(newConnection);
}

void Data::sortCircle()
{
    if(mesh){
        mesh->sortCircle();
        changed = true;
    }
}

void Data::sortRow()
{
    if(mesh){
        mesh->sortRow();
        changed = true;
    }
}

void Data::sortForce()
{
    if(mesh){
        mesh->sortForce();
        changed = true;
    }
}

void Data::runMesh()
{
    if(!changed){
        framework->runMesh();
        runMode = true;
        emit runModeChanged();
    }
}

int Data::getFocusedID()
{
    return mesh->focusObject;
}

/**
 * @brief Data::removeNodeFromMesh removes node with given ID from mesh
 * @param ID
 */
void Data::removeNodeFromMesh(int ID) {
    if (!mesh->nodesInMesh.contains(ID)) return;
    mesh->deleteNode(ID);
    changed = true;
}

bool Data::checkConnection(int srcNodeID, QString srcGate, int destNodeID, QString destGate)
{
    if(mesh)
        return mesh->checkConnection(srcNodeID, srcGate, destNodeID, destGate);
    else return false;
}

bool Data::hasChanged()
{
    return changed;
}

void Data::unsetChanged()
{
    changed = false;
}

bool Data::mustRepaint()
{
    return repaint;
}

void Data::unsetRepaint()
{
    repaint = false;
}

void Data::setChanged()
{
    changed = true;
    repaint = true;
}

Node *Data::getNodeByName(QString name)
{
    if(mesh)
        return mesh->getNodeByName(name);
    return 0;
}

Node *Data::getNodeByID(int ID)
{
    if(mesh)
        return mesh->getNodeByID(ID);
    return 0;
}

QList<Node *> Data::getAllNodes()
{
    QList<Node *> result;
    if(mesh)
        result = mesh->getAllNodes();
    return result;
}

QList<Connection *> Data::getAllConnections()
{
    QList<Connection *> result;
    if(mesh)
        result = mesh->getAllConnections();
    return result;
}

QMap<int, Node *> *Data::nodesInMesh()
{
    if(mesh)
        return &mesh->nodesInMesh;
    return 0;
}

QString Data::getValidAlternativeForName(QString &name)
{
    QString result;
    if(mesh)
        result = mesh->getValidAlternativeForName(name);
    return result;
}

/**
 * @brief Data::moveObjectInMesh moves drwan object with given id from start to end
 * @param start start point
 * @param end end point
 * @param ID ID of moving item
 */
void Data::moveObjectInMesh(QPoint *start, QPoint *end, int ID) {
    // is object a node?
    if (this->mesh->nodesInMesh.contains(ID)) {
        this->moveObjectInMesh(end, ID);
        changed = true;
    }
    // is it a connection?
    else if (this->mesh->connectionsInMesh.contains(ID)) {
        int joint = this->mesh->getConnectionByID(ID)->getJoint(start);
        // this is just a temp. solution!
        this->mesh->getConnectionByID(ID)->setJoint(joint, end);
        changed = true;
        SingletonRender::instance()->renderMesh();
    }
}

/**
 * @brief Data::moveObjectInMesh
 * @param Position
 * @param ID
 * @see Data::moveObjectInMesh(QPoint*,QPoint*,int)
 */
void Data::moveObjectInMesh(QPoint *Position, int ID) {
    if (this->mesh->nodesInMesh.contains(ID)) {
        this->mesh->getNodeByID(ID)->moveTo(Position->x(), Position->y());
        changed = true;
        SingletonRender::instance()->renderMesh();
    } else if (this->mesh->connectionsInMesh.contains(ID)) {
        qDebug() << "cant move a connection joint with onle one QPoint";
        return;
    }
}

Mesh *Data::getMesh() { return this->mesh; }

void Data::newMeshProject() {
    //create new mesh and delete old
    delete mesh;
    mesh = new Mesh();

    //clear the mesh field
    SingletonRender::instance()->clearMeshField();
    mainWindow->ui->meshField->connectSignals();
    //render the new mesh
    SingletonRender::instance()->renderMesh();
}

bool Data::deleteItem() {
    bool deleted = this->mesh->deleteItem();
    if(deleted)
        changed = true;
    return deleted;
}

/**
 * @brief Data::~Data deletes mesh, nodeCatalog & the singelton object
 */
Data::~Data() {
    delete mesh;
    delete nodeCatalog;
    delete data;
}

NodeCatalog *Data::getNodeCatalog() { return nodeCatalog; }

void Data::setDrawLineMode(QString gateType){

    this->mesh->setFocusMeshObject(-1);
    SingletonRender::instance()->highlightGates(gateType);
}

bool Data::isSimulating()
{
    return onSimulation;
}

void Data::setFocusMeshObject(int nodeID)
{
    if(mesh)
        mesh->setFocusMeshObject(nodeID);
}

void Data::updateNode(QTableWidgetItem *item)
{
    if(mesh)
        mesh->updateNode(item);
}

void Data::startSimulation()
{
    onSimulation = true;
}

void Data::stopSimulation()
{
    onSimulation = false;
}

void Data::setEditMode(){
    SingletonRender::instance()->dehighlightGates();
    runMode = false;
    emit runModeChanged();
}

QList<Connection *> Data::getConnections(int nodeID){
    QList<Connection *> result;
    for(Connection *c : mesh->connectionsInMesh.values())
        if(c->getSrcNode()->getID() == nodeID || c->getDestNode()->getID() == nodeID)
            result << c;
    return result;
}
