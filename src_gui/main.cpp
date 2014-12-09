#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    MainWindow window;
    window.show();

    //read a json file:
    FileHandler filreaderObj;
    QDir directory = QDir::current();   //den pfad basteln
    directory.cdUp();
    QString path = (directory.path()).append("/Data/jsondatei.json");

    filreaderObj.loadFile(path); //mit dem richtigen pfad aufgerufen
    filreaderObj.printFile();   //ausgabe der datei die man gelesen hat

    return application.exec();
}
