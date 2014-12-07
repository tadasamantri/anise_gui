#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    MainWindow window;
    window.show();

    return application.exec();
}
