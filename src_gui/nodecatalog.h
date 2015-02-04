#ifndef NODECATALOG_H
#define NODECATALOG_H

#include "node.h"
#include <QMap>
#include <QDebug>

/*
 * this class does not do any graphics stuff. It's part of the data structure!
 */
class NodeCatalog {
public:

    QMap<QString, Node> Content;
    void insert(Node nodeToInsert);
    Node getNodeOfType(const QString &type);
    NodeCatalog();





};

#endif  // NODECATALOG_H
