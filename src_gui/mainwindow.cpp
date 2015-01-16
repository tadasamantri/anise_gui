#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QVector>
#include <QCursor>
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


    initializeGUI();





    // ##### generate a node just for testing purpose
    //later nodes should be created by reading from a json file


        Node *tempTestNode = new Node(); //using standart construktor
        //setting individual variables

        //name and type
        tempTestNode->setType("TestType");
        tempTestNode->setName("Dieter");

        //generate and add test gates:
        //input
        QVector<QString> *tempTestGateInputTypes = new QVector<QString>();
        tempTestGateInputTypes->append("type1");
        tempTestGateInputTypes->append("type2");
        tempTestGateInputTypes->append("type3");
        //output
        QVector<QString> *tempTestGateOutputTypes = new QVector<QString>();
        tempTestGateInputTypes->append("type4");
        tempTestGateInputTypes->append("type1");
        tempTestGateInputTypes->append("type5");

        Gate *tempTestGateInput = new Gate(true, *tempTestGateInputTypes);
        Gate *tempTestGateOutput = new Gate(false, *tempTestGateOutputTypes);
        tempTestNode->addGate(*tempTestGateInput,1);
        tempTestNode->addGate(*tempTestGateOutput,0);

        //add parameter
        tempTestNode->addParam("param1", 1);
        tempTestNode->addParam("param2", false);
        tempTestNode->addParam("param3", 19.4f);
        tempTestNode->addParam("param4", 3.141);

        //set the position of the Node
        tempTestNode->setPosition(1.0 , 1.0);

    // ##### end of generation of testing node

    //now we want to render this Node with the render class:
    //RenderClass testRenderer = new

    NodeCatalog_Render(tempTestNode);






}

void MainWindow::initializeGUI(){

    //general initialization of the mainwindow
    ui->setupUi(this);//generate gui from xml
    menuBar()->setNativeMenuBar(false); //schaltet natives ubuntu menu aus
    new Q_DebugStream(std::cout, ui->qDebug_out); //leitet qDebug ins logfenster
    Q_DebugStream::registerQDebugMessageHandler();

    //initalize stored Settings
    SettingsHandler::setSettingsPath(QApplication::applicationDirPath() + "/settings.ini");
    //qDebug() << QApplication::applicationDirPath() + "/settings.ini";

    //Check if Framework path is set

    if(SettingsHandler::contains("frameworkpath"))
        AniseCommunicator::setFrameworkPath(SettingsHandler::loadSetting("frameworkpath"));
    else{

        /*
         * TODO different outcome of buttons
         * right now whatever you click will result in the same outcome
         */
        QMessageBox::information(0, QString("Please, set your framework path"), QString("You haven't set your framework path yet.\nChoose first!"), "Choose", "Not yet");


        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "Set your framework path",
                                                        "",
                                                        "(anids_framework (*)");

       SettingsHandler::storeSetting("frameworkpath", fileName);

    }

    //initialize settings from .ini file
    SettingsHandler::initializeSettings();

    //START LOADING NODE TYPES

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





void MainWindow::NodeCatalog_Render(Node* nodeToRender){

    //temporary render method. should be later moved into the render class
    qDebug() << "render method in mainwindow reached";
    RenderClass *A = new RenderClass() ;

    //render in the side
    A->renderNode(nodeToRender,ui->nodeCatalogContent);

    //render in the main window
    //qDebug() << "x: " << QCursor::pos().x() << " y: "<< QCursor::pos().y() ;
    //nodeToRender->setPosition(QCursor::pos().x(), QCursor::pos().y());
    A->renderNode(nodeToRender,ui->meshWorkWidget);


}

