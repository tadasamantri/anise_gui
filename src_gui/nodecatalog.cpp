#include "nodecatalog.h"

NodeCatalog *NodeCatalog::m_pInstance = NULL;

NodeCatalog::NodeCatalog() {
    Content = QMap<QString, Node>();
}


void NodeCatalog::insert(Node nodeToInsert) {
    QString type = nodeToInsert.getType();
    this->Content[type] = nodeToInsert;
}

Node NodeCatalog::getNodeOfType(const QString &type){
    Node node;
    node = Content[type];
    return node;
}

NodeCatalog *NodeCatalog::instance()
{
    if (!m_pInstance)  // Only allow one instance of class to be generated.
        m_pInstance = new NodeCatalog();

    return m_pInstance;
}
