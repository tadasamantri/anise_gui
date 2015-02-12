#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMouseEvent>
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QAbstractButton>
#include "jsonfilehandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    // Ui::MainWindow *getUi();
    ~MainWindow();
    Ui::MainWindow *ui;

    void updatePropertyTable(int nodeID);
    void displayTypeInfo(const QString &type);
public slots:

private slots:

    // Action for loading .mesh files
    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionSet_framework_path_triggered();

    void on_actionNew_triggered();

    void on_actionLoad_Catalog_triggered();

private:
    void initializeGUI();
    void deleteTable();
};

#endif  // MAINWINDOW_H
