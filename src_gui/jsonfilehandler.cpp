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
 * @brief JsonFileHandler::readFile converts a textfile into a QJsonDocument-Object
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
 * @brief JsonFileHandler::parseNodeTypesFromAnise fills the nodeCatalog with Node Templates
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

        QJsonObject localNode = var.toObject();
        type = localNode["class"].toString();
        descr = localNode["description"].toString();
        QJsonArray inputs = localNode["input_gates"].toArray(),
                outputs = localNode["output_gates"].toArray();

        if (!inputs.isEmpty()) foreach (QJsonValue value, inputs) {
            QJsonObject localGate = value.toObject();
            QString gateName = localGate["name"].toString(),
                    gateType = localGate["type"].toString();
            Gate* gate = new Gate();
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
        catalog->insert(node);
        qDebug() << "added node to Catalog:\n"
                 << "class: " << node.getType()
                 << "\ninputs: " << input_gates.size()
                 << "\noutputs: " << output_gates.size();
    }
}

/**
 * @brief JsonFileHandler::extractNodesAndConnections Extracts all Nodes and Connections of a QJsonObject
 * @param obj the QJsonObject containing the JSON File
 * @param nodelist List in which the nodes will be written
 * @param connectionlist List in which the connections will be written
 */
void JsonFileHandler::extractNodesAndConnections(
        /*input*/ const QJsonObject &obj, /*output*/ QList<Node *> &nodelist,
        /*output*/ QList<Connection *> &connectionlist) {
    // check if there are any nodes
    int i = 1, j = 1;  // for debug only
    if (!obj["nodes"].isArray()) {
        qWarning() << "no nodes in JSON-Object";
        return;
    }

    qDebug() << "File contains nodes, so let's parse them";

    //QMap<QString, Node*> nodemap;
    // for every node (represented as jsonvalue)...
    foreach (QJsonValue var, obj["nodes"].toArray()) {
        //...convert it to json object...
        QJsonObject theNode = var.toObject();

        // check if nodes are declared correctly
        if (!(theNode["class"].isString() && theNode["name"].isString() &&
              theNode["params"].isArray()))
            qWarning() << "Error while extracting node:\n" << theNode;

        // node is welldefined =)
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
            nodelist << createdNode;
        }
    }

    // nodes parsed
    //nodelist = nodemap.values();

    if (!obj["connections"].isArray()) {
        qDebug() << "no connections found";
        return;
    }

    foreach (QJsonValue var, obj["connections"].toArray()) {
        QVariantMap theConnection = var.toObject().toVariantMap();
        Node *src_node = findNodeByName(&nodelist,theConnection["src_node"].toString());
        Node *dest_node = findNodeByName(&nodelist, theConnection["dest_node"].toString());
        Connection *connection = new Connection(src_node, src_node->getGateByName(theConnection["src_gate"].toString()), dest_node,  dest_node->getGateByName(theConnection["dest_gate"].toString()));
        connectionlist << connection;
    }
}

/**
 * @brief JsonFileHandler::findNodeByName searches Node with special name in given list
 * @param nodes
 * @param name
 * @return node if found, NULL else
 */
Node *JsonFileHandler::findNodeByName(const QList<Node*> *nodes, const QString &name){
    Node *node;
    for(int i = 0; i < nodes->size(); i++){
        node = nodes->at(i);
        if(node->getName()==name)
            return node;
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

QString *JsonFileHandler::meshToJson(Mesh *mesh) {
    QString *jsonString = new QString();

    *jsonString += "{\"nodes: [";

    foreach (Node *localNode, mesh->getAllNodes()) {
        *jsonString += "{\"class\": " + localNode->getType() + "\",";
        *jsonString += " \"name\": \"" + localNode->getName() + "\", ";
        *jsonString += " \"params\": [";
        foreach (QString key, localNode->params.keys()) {
            *jsonString +=
                    "{\"" + key + "\": \"" + localNode->params[key].toString() + "\"},";
        }
        // remove obsolete last ","
        if (*(jsonString->end()) == ',') jsonString->chop(1);
        *jsonString += "]},";
    }
    // remove obsolete last ","
    if (*(jsonString->end()) == ',') jsonString->chop(1);
    *jsonString += "],";

    *jsonString += "\"connections\": [";


    /* TODO: problem with gates. maybe not created properly? */
    foreach (Connection *localConnection, mesh->getAllConnections()) {
        Node *src = localConnection->getSrcNode(),
                *dst = localConnection->getDestNode();
        *jsonString += "{\"src_node\": \"";
        *jsonString += src->getName();
        *jsonString += "\", \"src_gate\": \"";
        *jsonString += localConnection->getSrcGate()->getName();
        *jsonString += "\", ";
        *jsonString += "\"dest_node\": \"";
        *jsonString += dst->getName();
        *jsonString += "\", \"dest_gate\": \"";
        *jsonString += localConnection->getDestGate()->getName();
        *jsonString += "\"},";
    }
    // remove obsolete last ","
    if (*(jsonString->end()) == ',') jsonString->chop(1);
    *jsonString += "]}";
    return jsonString;
}
