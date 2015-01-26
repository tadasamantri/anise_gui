#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QVector>
#include <QCursor>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "anisecommunicator.h"
#include "settingshandler.h"
#include "qdebugstreamredirector.h"
#include "mesh.h"
#include "singletonrender.h"
#include "nodecatalog.h"
#include "data.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    //Important to do first! SingletonRender is used in initialize GUI
    SingletonRender::instance()->setUi(this->ui);

    initializeGUI();
/*
    // create the shortcut after the list widget has been created

    // option A (pressing DEL anywhere in the main window activates the slot)
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(deleteItem()));

    // option B (pressing DEL activates the slots only when list widget has focus)
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), listWidget);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteItem()));
*/
}

void MainWindow::initializeGUI() {
    // general initialization of the mainwindow
    ui->setupUi(this);                   // generate gui from xml
    menuBar()->setNativeMenuBar(false);  // schaltet natives ubuntu menu aus
    // new Q_DebugStream(std::cout, ui->qDebug_out); // leitet qDebug ins
    // logfenster
    // Q_DebugStream::registerQDebugMessageHandler();

    // make the mesh editor accept drops
    ui->mesh_edt_area->setAcceptDrops(true);

    //initialize all data content
    Data::instance()->initialize(this);

}
/*
Ui::MainWindow MainWindow::getUi(){
    return this->ui;

}*/

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionLoad_triggered() {
    qDebug() << "Trying to open FileDialog";
    QString fileName = QFileDialog::getOpenFileName(
                this, "Load previously saved mesh", "",
                "Mesh-Files (*.mesh *.json);; All Files (*.*)");

    qDebug() << "Path to File loaded\nPath is" << fileName
             << "\nnow let's load it to the FileHandler!";

    if(fileName == "")
        return;
    QList<Node> nodes;
    QList<Connection> connections;
    QJsonObject *obj = JsonFileHandler::readFile(fileName);
    JsonFileHandler::extractNodesAndConnections(*obj, nodes, connections);
    //Mesh mesh;
    //mesh.addNodes(nodes);
    //mesh.addConnections(connections);

}

void MainWindow::on_actionSet_framework_path_triggered() {
    qDebug() << "Try to open Folder Chooser";
    QFileDialog dialog(this);

    QString fileName =
            QFileDialog::getOpenFileName(this, "Set your framework path", "", "");

    SettingsHandler::storeSetting("frameworkpath", fileName);
    AniseCommunicator::setFrameworkPath(fileName);
}

void MainWindow::on_actionNew_triggered() {

    //TODO ask if old is wished to thrown away if not saved
    //qDebug() << "Mesh before NEW:\t Nodes: " << Data::instance()->mesh->getAllNodes().size();
    Data::instance()->newMeshProject();
    //qDebug() << "Mesh after NEW:\t Nodes: " << Data::instance()->mesh->getAllNodes().size();
}

void MainWindow::on_actionLoad_Catalog_triggered()
{
    QString out = AniseCommunicator::getAllNodeTypes();
    JsonFileHandler::parseNodeTypesFromAnise(out);
}
