#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include "anisecommunicator.h"
#include "settingshandler.h"
#include "qdebugstreamredirector.h"

#include "mesh.h"


#include "renderclass.h"
#include "nodecatalog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //general initialization of the mainwindow
    ui->setupUi(this);//generate gui from xml
    menuBar()->setNativeMenuBar(false); //schaltet natives ubuntu menu aus
    new Q_DebugStream(std::cout, ui->qDebug_out); //leitet qDebug ins logfenster
    Q_DebugStream::registerQDebugMessageHandler();

    //initalize stored Settings
    SettingsHandler::setSettingsPath(QApplication::applicationDirPath() + "/settings.ini");
    qDebug() << QApplication::applicationDirPath() + "/settings.ini";

    //Check if Framework path is set

    if(SettingsHandler::contains("frameworkpath"))
        AniseCommunicator::setFrameworkPath(SettingsHandler::loadSetting("frameworkpath"));
    else{
         QMessageBox::information(0, QString("Please, set your framework path"), QString("You haven't set your framework path yet.\nChoose first!"), "Choose", "Not yet");

        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "Set your framework path",
                                                        "",
                                                        "(anids_framework (*)");

       SettingsHandler::storeSetting("frameworkpath", fileName);

    }

    SettingsHandler::initializeSettings();
    NodeCatalog_Render();

    //load all available NodeTypes
    AniseCommunicator::getAllNodeTypes();




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered()
{
    qDebug() << "Trying to open FileDialog";
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Load previously saved mesh",
                                                    "",
                                                    "Mesh-Files (*.mesh *.json);; All Files (*.*)");

    qDebug() <<   "Path to File loaded\nPath is"
               << fileName
                  << "\nnow let's load it to the FileHandler!";

    QString fileStringContent =  JsonFileHandler::loadFile(fileName);
    JsonFileHandler::printFile(fileStringContent); //only for debugging
    JsonFileHandler::parseJsonString(fileStringContent);
}

void MainWindow::on_actionSet_framework_path_triggered()
{
    qDebug() << "Try to open Folder Chooser";
              QFileDialog dialog(this);


              QString fileName = QFileDialog::getOpenFileName(this,
                                                              "Set your framework path",
                                                              "",
                                                              "(All Files (anids-framework)");

              SettingsHandler::storeSetting("frameworkpath", fileName);
              AniseCommunicator::setFrameworkPath(fileName);

}


void MainWindow::on_actionNew_triggered()
{

    Mesh newMesh();


}


void MainWindow::NodeCatalog_Render(){

   qDebug() << "render method in mainwindow reached";
   RenderClass *A = new RenderClass(ui->nodeCatalogContent) ;
   ui->nodeCatalogContent->setMinimumWidth(A->Nodes[0]->height() + 15);
    ui->nodeCatalogContent->setMinimumHeight((A->Nodes.size() )  * A->Nodes[0]->height());
   for (int i = 0; i < A->Nodes.size(); ++i) {
        A->Nodes.at(i)->show();

   }


}

