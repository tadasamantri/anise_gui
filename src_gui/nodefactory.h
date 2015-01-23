#ifndef NODEFACTORY_H
#define NODEFACTORY_H
#include "node.h"
#include <QVariantMap>
#include <QDebug>
#include "nodecatalog.h"


class NodeFactory { //singleton
public:

    static NodeFactory *instance();
    static Node createNode();
    static Node createNode(QString _class);
    static Node createNode(QString _class, QString name);
    static Node createNode(QString _class, QString name, QVariantMap params);
    static Node *createTestNode();


private:
    NodeFactory();
    // copy constructor is private
    NodeFactory(NodeFactory const &){}

    // assignment operator is private
    NodeFactory &operator=(NodeFactory const &){}

    // pointer to this instance
    static NodeFactory *m_pInstance;
};

#endif  // NODEFACTORY_H
