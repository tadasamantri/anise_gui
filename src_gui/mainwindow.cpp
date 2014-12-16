#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "qdebugstreamredirector.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuBar()->setNativeMenuBar(false);
    new Q_DebugStream(std::cout, ui->qDebug_out);
    Q_DebugStream::registerQDebugMessageHandler();
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
    JsonFileHandler::parseFile(fileStringContent);
}
