#include <QApplication>
#include <QCoreApplication>
#include <QDir>

#include "mainwindow.h"
#include "singletonrender.h"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  MainWindow window;
  window.show();


  return application.exec();
}
