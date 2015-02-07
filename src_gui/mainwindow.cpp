#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QVector>
#include <QCursor>
#include <QShortcut>
#include <QSpinBox>
#include <limits>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "anisecommunicator.h"
#include "settingshandler.h"
#include "qdebugstreamredirector.h"
#include "mesh.h"
#include "singletonrender.h"
#include "nodecatalog.h"
#include "data.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Important to do first! SingletonRender is used in initialize GUI
    SingletonRender::instance()->setUi(this->ui);

    initializeGUI();
    /*
    // create the shortcut after the list widget has been created

    // option A (pressing DEL anywhere in the main window activates the slot)
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(deleteItem()));
*/
    // option B (pressing DEL activates the slots only when list widget has focus)
    // EDIT: works fine now =)
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);

    qDebug() << "FOCUSWINDOW: " << QApplication::focusWidget();
    connect(shortcut, SIGNAL(activated()), Data::instance(), SLOT(deleteItem()));
}

void MainWindow::initializeGUI() {
    // general initialization of the mainwindow
    ui->setupUi(this);                   // generate gui from xml
    menuBar()->setNativeMenuBar(false);  // schaltet natives ubuntu menu aus
    // new Q_DebugStream(std::cout, ui->qDebug_out); // leitet qDebug ins
    // logfenster
    // Q_DebugStream::registerQDebugMessageHandler();

    // make the mesh editor accept drops
    ui->mesh_edt_area->setAcceptDrops(true);

    // setup viewports
    ui->Node_Catalog->setupViewport(ui->nodeCatalogContent);
    ui->mesh_edt_area->setupViewport(ui->meshField);

    // hides the tab-widget
    ui->tabWidget->hide();

    // prepare propertytable
    ui->tableWidget->hide();
    ui->tableWidget->setColumnCount(2);
    // initialize all data content
    Data::instance()->initialize(this);
}
/*
Ui::MainWindow MainWindow::getUi(){
    return this->ui;

}*/

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionLoad_triggered() {
    qDebug() << "Trying to open FileDialog";
    QString fileName = QFileDialog::getOpenFileName(
                this, "Load previously saved mesh", "",
                "Mesh-Files (*.mesh *.json);; All Files (*.*)");

    qDebug() << "Path to File loaded\nPath is" << fileName
             << "\nnow let's load it to the FileHandler!";

    if (fileName == "") return;
    QJsonObject *obj = JsonFileHandler::readFile(fileName);
    JsonFileHandler::extractNodesAndConnections(*obj);
}

void MainWindow::on_actionSet_framework_path_triggered() {
    qDebug() << "Try to open Folder Chooser";
    QFileDialog dialog(this);

    QString fileName =
            QFileDialog::getOpenFileName(this, "Set your framework path", "", "");

    SettingsHandler::storeSetting("frameworkpath", fileName);
    AniseCommunicator::setFrameworkPath(fileName);
}

void MainWindow::on_actionNew_triggered() {
    // TODO ask if old is wished to thrown away if not saved
    // qDebug() << "Mesh before NEW:\t Nodes: " <<
    // Data::instance()->mesh->getAllNodes().size();
    Data::instance()->newMeshProject();
    // qDebug() << "Mesh after NEW:\t Nodes: " <<
    // Data::instance()->mesh->getAllNodes().size();
}

void MainWindow::on_actionLoad_Catalog_triggered() {
    QString out = AniseCommunicator::getAllNodeTypes();
    JsonFileHandler::parseNodeTypesFromAnise(out);
}

void MainWindow::on_buttonBox_clicked(QAbstractButton *button) {
    qDebug() << "hey man ich wurde aufgerufen:" << button;
}

void MainWindow::on_actionSave_triggered() {
    Mesh *theMesh = Data::instance()->getMesh();
    QString fileName =
            QFileDialog::getSaveFileName(this, "Save current project to...", "",
                                         "Mesh-Files (*.mesh *.json);;All Files(*)");
    QFile file(fileName);

    file.open(QIODevice::WriteOnly);
    // QByteArray data = *JsonFileHandler::meshToJson(theMesh);
    file.write(JsonFileHandler::meshToJson(theMesh).toUtf8());
    file.close();
}

void MainWindow::updatePropertyTable(int nodeID){
    QTableWidget *table = ui->tableWidget;

    if (nodeID >= 0 && Data::instance()->getMesh()->nodesInMash.contains(nodeID)) {
        Node *n = Data::instance()->getMesh()->getNodeByID(nodeID);
        QVariantMap *map = n->getParams();
        table->setRowCount(map->size() + 3);

        // create the entries all nodes have
        QTableWidgetItem *name = new QTableWidgetItem(),
                *ID = new QTableWidgetItem(),
                *type = new QTableWidgetItem();

        // set the data
        ID->setData(0, nodeID);
        name->setData(0, n->getName());
        type->setData(0, n->getType());

        // asign to table
        table->setItem(0, 0, new QTableWidgetItem("NodeID"));
        table->setItem(0, 1, ID);
        table->item(0, 0)->setFlags(table->item(0, 0)->flags() ^
                                    (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        table->setItem(1, 0, new QTableWidgetItem("Node Class"));
        table->setItem(1, 1, type);
        table->item(1, 0)->setFlags(table->item(1, 0)->flags() ^
                                    (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        table->setItem(2, 0, new QTableWidgetItem("Node Name"));
        table->setItem(2, 1, name);

        // make name editable
        name->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable |
                       Qt::ItemIsSelectable);

        // avoid editing keys, and nodeID/type
        ID->setFlags(ID->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable |
                                    Qt::ItemIsEnabled));
        type->setFlags(type->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable |
                                        Qt::ItemIsEnabled));
        for (int i = 0; i < 3; i++)
            table->item(i, 0)->setFlags(table->item(i, 0)->flags() ^
                                        (Qt::ItemIsEditable | Qt::ItemIsSelectable));
        QStringList keys = map->keys();
        for (int i = 0; i < keys.size(); i++) {
            // create key item
            QString key = keys.value(i);
            QTableWidgetItem *key_item = new QTableWidgetItem(key);
            key_item->setFlags(key_item->flags() ^
                               (Qt::ItemIsEditable | Qt::ItemIsSelectable));
            table->setItem(3 + i, 0, key_item);

            // create value item
            QVariant value = map->value(key);
            QTableWidgetItem *value_item;
            QSpinBox *spinner;
            switch (value.userType()) {
            case QVariant::Bool:
                value_item = new QTableWidgetItem();
                // value_item->setData(0, value);
                value_item->setCheckState(value.toBool() ? Qt::Checked
                                                         : Qt::Unchecked);
                table->setItem(3 + i, 1, value_item);
                break;
            case QVariant::Int:
                spinner = new QSpinBox(table);
                spinner->setMaximum(std::numeric_limits<int>::max());
                spinner->setMinimum(std::numeric_limits<int>::min());
                spinner->setValue(value.toInt());
                table->setCellWidget(3 + i, 1, spinner);
                break;
            case QVariant::UInt:
                spinner = new QSpinBox(table);
                spinner->setMaximum(std::numeric_limits<int>::max());
                spinner->setMinimum(0);
                spinner->setValue(value.toInt());
                table->setCellWidget(3 + i, 1, spinner);
                break;
            default:
                value_item = new QTableWidgetItem();
                value_item->setData(0, value);
                table->setItem(3 + i, 1, value_item);
                break;
            }
        }
        table->show();
    } else if (table->isVisible()){
        table->hide();
        qDebug() << "table destroyed";
        // delete all tableitems, because they aren't needed any more
        for (int col = 0; col < table->columnCount(); col++)
            for (int row = 0; row < table->rowCount(); row++)
                delete table->item(row, col);
        table->setRowCount(0);
    }
}
