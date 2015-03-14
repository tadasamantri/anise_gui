#ifndef NODECATALOG_H
#define NODECATALOG_H

#include "node.h"

/*
 * this class does not do any graphics stuff. It's part of the data structure!
 */
class NodeCatalog {
public:
    void insert(Node nodeToInsert);
    Node getNodeOfType(const QString &type);
    const Node *getPointerOfType(const QString &type);
    NodeCatalog();
    void clearContent();
    QMap<QString, Node> getContent();
    QVector<Node> getContentVector();
    QList<Node> getContentList();
private:
    QMap<QString, Node> Content;
};

#endif  // NODECATALOG_H
