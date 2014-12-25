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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuBar()->setNativeMenuBar(false);
    new Q_DebugStream(std::cout, ui->qDebug_out);
    Q_DebugStream::registerQDebugMessageHandler();

    //initalize stored Settings
    SettingsHandler::setSettingsPath(QApplication::applicationDirPath() + "/settings.ini");
    qDebug() << QApplication::applicationDirPath() + "/settings.ini";

    //Check if Framework path is set
    if(!SettingsHandler::contains("frameworkpath")){



        QMessageBox::information(0, QString("Please, set your framework path"), QString("You haven't set your framework path yet.\nChoose first!"), "Choose", "go to hell");
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "Set your framework path",
                                                        "",
                                                        "(anids_framework (*)");

       SettingsHandler::storeSetting("frameworkpath", fileName);
    }

    SettingsHandler::initializeSettings();

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
