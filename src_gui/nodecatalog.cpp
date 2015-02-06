#include "nodecatalog.h"
#include <QDebug>

NodeCatalog::NodeCatalog() {
    Content = QMap<QString, Node>();
    Node blackbox;
    Gate *in = new Gate(true, "in"), *out = new Gate(false, "out");
    blackbox.addGate(in);
    blackbox.addGate(out);
    blackbox.setType("blackbox");
    blackbox.setDescription("A Blackbox Node for all unknown Nodes");
    blackbox.addParam("boolean", true);
    blackbox.addParam("boolean2" ,false);
    blackbox.addParam("integer", -5);
    blackbox.addParam("String", "Test String");
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
