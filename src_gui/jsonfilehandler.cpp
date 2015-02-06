#include "jsonfilehandler.h"
#include "data.h"

QString JsonFileHandler::loadFile(const QString &path) {
    // QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QString fileContent = "";

    // open a file
    qDebug() << "trying to open: \n" << path << "\n";
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    // read each line of that file and append it to the String
    while (file.bytesAvailable()) {
        QByteArray line = file.readLine();
        fileContent.append(line);
    }

    file.close();
    return fileContent;
}

/**
 * @brief JsonFileHandler::readFile converts a textfile into a
 * QJsonDocument-Object
 * @param path contains path to a text file in json format
 * @return filecontent as QJsonObject
 */
QJsonObject *JsonFileHandler::readFile(const QString &path) {
    // read the file
    QJsonParseError jerror;
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll(), &jerror);
    file.close();
    //-------------------

    // was there an error?
    if (jerror.error != QJsonParseError::NoError) {
        qDebug() << jerror.errorString();
        // return NULL;
    }

    // no error
    QJsonObject *obj = new QJsonObject(jdoc.object());
    return obj;
}

/**
 * @brief JsonFileHandler::parseNodeTypesFromAnise fills the nodeCatalog with
 * Node Templates
 * @param output the output of the framework with parameters "--nodes --machine"
 */
void JsonFileHandler::parseNodeTypesFromAnise(QString &output) {
    if (output == "") return;

    QJsonDocument doc = QJsonDocument::fromJson(output.toUtf8());

    NodeCatalog *catalog = Data::instance()->getNodeCatalog();
    qDebug() << "trying to read all node types. Loading json data...\n";

    QJsonObject obj = doc.object();

    if (!obj["nodes"].isArray()) {
        qWarning() << "no nodes in JSON-Object";
        return;
    }

    foreach (QJsonValue var, obj["nodes"].toArray()) {
        QString type, descr;
        QList<Gate *> input_gates, output_gates;

        QJsonObject localNode = var.toObject(),
                json_params;
        type = localNode["class"].toString();
        descr = localNode["description"].toString();
        QJsonArray inputs = localNode["input_gates"].toArray(),
                outputs = localNode["output_gates"].toArray();

        if (!inputs.isEmpty()) foreach (QJsonValue value, inputs) {
            QJsonObject localGate = value.toObject();
            QString gateName = localGate["name"].toString(),
                    gateType = localGate["type"].toString();
            Gate *gate = new Gate();
            gate->setDirection(true);
            gate->setName(gateName);
            gate->addType(gateType);
            input_gates << gate;
        }
        if (!outputs.isEmpty()) foreach (QJsonValue value, outputs) {
            QJsonObject localGate = value.toObject();
            QString gateName = localGate["name"].toString(),
                    gateType = localGate["type"].toString();
            Gate *gate = new Gate();
            gate->setDirection(false);
            gate->setName(gateName);
            gate->addType(gateType);
            output_gates << gate;
        }
        Node node;
        node.setType(type);
        node.setDescription(descr);
        node.addGates(input_gates.toVector(), true);
        node.addGates(output_gates.toVector(), false);

        //initilize parameters
        json_params = localNode["parameters"].toObject();
        QVariantMap contents = localNode.toVariantMap()["parameters"].toMap();
        foreach (QString key, contents.keys()) {
            QVariant::Type type = static_cast<QVariant::Type> (contents[key].toInt());
            node.addParam(key, QVariant(type));


        }
        catalog->insert(node);
        qDebug() << "added node to Catalog:\n"
                 << "class: " << node.getType()
                 << "\ninputs: " << input_gates.size()
                 << "\noutputs: " << output_gates.size();
    }
}

/**
 * @brief JsonFileHandler::extractNodesAndConnections Extracts all Nodes and
 * Connections of a QJsonObject
 * @param obj the QJsonObject containing the JSON File
 * @param nodelist List in which the nodes will be written
 * @param connectionlist List in which the connections will be written
 */
void JsonFileHandler::extractNodesAndConnections(const QJsonObject &obj) {
    // check if there are any nodes
    int i = 1, j = 1;  // for debug only
    if (!obj["nodes"].isArray()) {
        qWarning() << "no nodes in JSON-Object";
        return;
    }

    qDebug() << "File contains nodes, so let's parse them";
    Mesh *mesh = Data::instance()->getMesh();

    // for every node (represented as jsonvalue)...
    foreach (QJsonValue var, obj["nodes"].toArray()) {
        //...convert it to json object...
        QJsonObject theNode = var.toObject();

        // check if nodes are declared correctly
        if (!(theNode["class"].isString() && theNode["name"].isString() &&
              theNode["params"].isArray()))
            qWarning() << "Error while extracting node:\n" << theNode;

        // node is welldefined:
        else {
            // get name and class(type) of node
            QString type = theNode["class"].toString(),
                    name = theNode["name"].toString();
            qDebug() << "node " << i++ << " parsed:\n name = " << name
                     << " | type = " << type;
            qDebug() << "params:";

            // ok, parameters are quite more difficult
            QVariantMap params;

            // get parameters as array of objects
            foreach (QJsonValue local, theNode["params"].toArray()) {
                // make every object a qvariantmap
                QVariantMap map = local.toObject().toVariantMap();
                // insert all records into params map

                foreach (QString key, map.keys()) {
                    params[key] = map[key];
                    qDebug() << j++ << ": " << key << " = " << params[key];
                }
            }
            j = 1;  // for debugging only
            qDebug() << "\n";

            // node is complete, so let's insert it
            Node *createdNode = NodeFactory::createNode(type, name, params);
            mesh->addNode(createdNode);
        }
    }

    // nodes parsed
    if (!obj["connections"].isArray()) {
        qDebug() << "no connections found";
        return;
    }

    foreach (QJsonValue var, obj["connections"].toArray()) {
        QVariantMap theConnection = var.toObject().toVariantMap();

        Node *src_node = mesh->getNodeByName(theConnection["src_node"].toString()),
                *dest_node =
                mesh->getNodeByName(theConnection["dest_node"].toString());
        Connection *connection = new Connection(
                    src_node, src_node->getGateByName(theConnection["src_gate"].toString()),
                dest_node,
                dest_node->getGateByName(theConnection["dest_gate"].toString()));

        mesh->addConnection(connection);
    }
}

/**
 * @brief JsonFileHandler::findNodeByName searches Node with special name in
 * given list
 * @param nodes
 * @param name
 * @return node if found, NULL else
 */
Node *JsonFileHandler::findNodeByName(const QList<Node *> *nodes,
                                      const QString &name) {
    Node *node;
    for (int i = 0; i < nodes->size(); i++) {
        node = nodes->at(i);
        if (node->getName() == name) return node;
    }
    return 0;
}

/**
 * @brief JsonFileHandler::writeFile writes a string into a given file
 * @param path path to file
 * @param fileContent the content written to the specified file
 */
void JsonFileHandler::writeFile(const QString &path,
                                const QString &fileContent) {
    QString command = "touch";
    QProcess touch;
    QStringList arg;
    arg << path;
    touch.start(command, arg);
    touch.waitForFinished(3000);
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << fileContent;
    file.close();
}

QString JsonFileHandler::meshToJson(Mesh *mesh) {
    QJsonArray nodes, connections;
    foreach (Node *n, mesh->getAllNodes()) {
        QJsonObject theNode;
        theNode["class"] = n->getType();
        theNode["name"] = n->getName();
        QJsonArray params;
        QVariantMap *map = n->getParams();
        foreach (QString key, map->keys()) {
            QJsonObject param;
            QVariant var = map->value(key);
            param[key] = QJsonValue::fromVariant(var);
            // TODO gebuggt!
            params.push_back(param);
        }
        theNode["params"] = params;
        nodes.push_back(theNode);
    }
    foreach (Connection *c, mesh->getAllConnections()) {
        QJsonObject theConnection;
        theConnection["src_node"] = c->getSrcNode()->getName();
        theConnection["src_gate"] = c->getSrcGate()->getName();
        theConnection["dest_node"] = c->getDestNode()->getName();
        theConnection["dest_gate"] = c->getDestGate()->getName();
        connections.push_back(theConnection);
    }
    QJsonObject obj;
    obj["connections"] = connections;
    obj["nodes"] = nodes;
    QJsonDocument doc;
    doc.setObject(obj);
    return doc.toJson();
}
