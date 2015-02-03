#include "nodecatalog.h"

NodeCatalog *NodeCatalog::m_pInstance = NULL;

NodeCatalog::NodeCatalog() {
    Content = QMap<QString, Node>();
    Node blackbox;
    Gate *in = new Gate(true, "in"), *out = new Gate(false, "out");
    blackbox.addGate(in);
    blackbox.addGate(out);
    blackbox.setType("blackbox");
    blackbox.setDescription("A Blackbox Node for all unknown Nodes");
    Content["blackbox"] = blackbox;
}


void NodeCatalog::insert(Node nodeToInsert) {
    QString type = nodeToInsert.getType();
    this->Content[type] = nodeToInsert;
}

Node NodeCatalog::getNodeOfType(const QString &type){
    Node node;
    QString _class = type;
    if(!Content.contains(_class))
        _class = "blackbox";
    node = Content[_class];
    qDebug() << "node of type " << _class <<" returned";
    return node;
}

NodeCatalog *NodeCatalog::instance()
{
    if (!m_pInstance)  // Only allow one instance of class to be generated.
        m_pInstance = new NodeCatalog();

    return m_pInstance;
}
