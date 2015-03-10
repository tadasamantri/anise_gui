#include <QApplication>
#include <QCoreApplication>
#include <QDir>

#include "mainwindow.h"
#include "singletonrender.h"

int main(int argc, char *argv[]) {
    int exitCode = 0;
    do{
        QApplication application(argc, argv);
        MainWindow window;
        window.show();
        exitCode = application.exec();
    }while(exitCode == MainWindow::EXIT_CODE_REBOOT);
    return exitCode;
}
