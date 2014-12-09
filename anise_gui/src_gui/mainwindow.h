#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>
#include "FileReader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
