#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "mesh.h"
#include "nodecatalog.h"
#include "mainwindow.h"
#include "data.h"
#include "renderclass.h"
#include "ui_mainwindow.h"

class Data : public QObject {
  Q_OBJECT
public:
  explicit Data(QObject *parent = 0);
  Mesh *mesh;
  NodeCatalog *nodeCatalog;
  Ui::MainWindow *ui;

  void setUi(Ui::MainWindow *ui);
  void addNodeToMesh(Node *newNode);
  void moveNodeInMesh(QPoint *Position, int numberOfNode);
};

#endif // DATA_H
