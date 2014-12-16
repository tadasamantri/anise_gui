#include "nodefactory.h"

NodeFactory::NodeFactory()
{

}
Node NodeFactory::createNode()
{
    Node node = Node();
    return node;
}

Node NodeFactory::createNode(const QString &_class)
{
    Node node = createNode();
    node.setType(_class);
    return node;
}

Node NodeFactory::createNode(const QString &_class, const QString &name)
{
    Node node = createNode(_class);
    node.setName(name);
    return node;
}

Node NodeFactory::createNode(const QString &_class, const QString &name, const QMap<QString,QString> &params)
{
    Node node = createNode(_class, name);
    foreach (QString key, params.keys()) {
        node.addParam(key,params.find(key).value());
    }
    return node;
}
