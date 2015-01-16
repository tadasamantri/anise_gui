#ifndef NODECATALOG_H
#define NODECATALOG_H

#include"node.h"
#include <QVector>



class NodeCatalog
{
public:
    NodeCatalog();
    QVector<Node> Content;
    void insert(Node *nodeToInsert);
};

#endif // NODECATALOG_H
