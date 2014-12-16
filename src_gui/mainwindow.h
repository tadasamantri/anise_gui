#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>
#include "jsonfilehandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private slots:

    //Action for loading .mesh files
    void on_actionLoad_triggered();

    void on_actionSet_framework_path_triggered();


private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
