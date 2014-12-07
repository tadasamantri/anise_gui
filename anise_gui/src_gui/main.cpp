#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    MainWindow window;
    window.show();

    FileReader filreaderObj;
    QString path = "jsondatei.json";
    filreaderObj.loadFile(path);
    filreaderObj.printFile();

    return application.exec();
}
