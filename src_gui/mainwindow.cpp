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
#include "dataholder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  //
  initializeGUI();
  DataHolder::getData()->setUi(this->ui);
  qDebug() << this->ui->mesh_edt_area->parent();
}

void MainWindow::initializeGUI() {

  // general initialization of the mainwindow
  ui->setupUi(this); // generate gui from xml
  menuBar()->setNativeMenuBar(false); // schaltet natives ubuntu menu aus
  new Q_DebugStream(std::cout, ui->qDebug_out); // leitet qDebug ins logfenster
  Q_DebugStream::registerQDebugMessageHandler();

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

    QString fileName = QFileDialog::getOpenFileName(
        this, "Set your framework path", "", "(anids_framework (*)");

    SettingsHandler::storeSetting("frameworkpath", fileName);
  }

  // initialize settings from .ini file
  SettingsHandler::initializeSettings();

    //create the Node catalog
    NodeCatalog TypeCatalog;
    //create the render object
    singletonRender::Instance();




    //create 20 test nodes
    for (int i = 0; i < 20; ++i) {

        Node *tempTestNode = NodeFactory::createTestNode();
        TypeCatalog.insert(tempTestNode);
    }
    //render the node catalog filled with test nodes
    singletonRender::Instance()->renderCatalogContent(TypeCatalog.Content,ui->nodeCatalogContent);

  // START LOADING NODE TYPES


  // load all available NodeTypes
  AniseCommunicator::getAllNodeTypes();

  // make the mesh editor accept drops
  ui->mesh_edt_area->setAcceptDrops(true);
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

  QString fileStringContent = JsonFileHandler::loadFile(fileName);
  JsonFileHandler::printFile(fileStringContent); // only for debugging
  JsonFileHandler::parseJsonString(fileStringContent);
}

void MainWindow::on_actionSet_framework_path_triggered() {
  qDebug() << "Try to open Folder Chooser";
  QFileDialog dialog(this);

  QString fileName = QFileDialog::getOpenFileName(
      this, "Set your framework path", "", "(All Files (anids-framework)");

  SettingsHandler::storeSetting("frameworkpath", fileName);
  AniseCommunicator::setFrameworkPath(fileName);
}

void MainWindow::on_actionNew_triggered() { Mesh newMesh(); }
