#include <QApplication>
#include <QCoreApplication>
#include <QDir>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    int exitCode = 0;
    QApplication application(argc, argv);
    do {
        MainWindow window;
        window.show();
        exitCode = application.exec();
    } while (exitCode == MainWindow::EXIT_CODE_REBOOT);
    return exitCode;
}
