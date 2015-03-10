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
     void deleteTable();
    void updatePropertyTable(int nodeID);
    void displayTypeInfo(const QString &type);
    static int const EXIT_CODE_REBOOT;
public slots:

private slots:

    // Action for loading .mesh files
    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionSet_framework_path_triggered();

    void on_actionNew_triggered();

    void on_actionLoad_Catalog_triggered();

    void on_details_stateChanged(int arg1);

private:
    void initializeGUI();
    bool pathChanged = false;
    QString saveDialog();
    void saveFile(QString &path);
protected:
    void closeEvent(QCloseEvent *event);
};

#endif  // MAINWINDOW_H
