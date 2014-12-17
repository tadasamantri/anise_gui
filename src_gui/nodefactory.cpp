#include "nodefactory.h"

NodeFactory::NodeFactory()
{

}
Node NodeFactory::createNode()
{
    Node node = Node();
    return node;
}

Node NodeFactory::createNode(QString _class)
{
    Node node = createNode();
    node.setType(_class);
    return node;
}

Node NodeFactory::createNode(QString _class, QString name)
{
    Node node = createNode(_class);
    node.setName(name);
    return node;
}

Node NodeFactory::createNode(QString _class, QString name, QVariantMap params)
{
    Node node = createNode(_class, name);
    foreach (QString key, params.keys()) {
        node.addParam(key,params.find(key).value());
    }
    return node;
}
