#include "jsonfilehandler.h"

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
    NodeCatalog *catalog = NodeCatalog::instance();
    qDebug() << "trying to read all node types. Loading json data...\n";


    QJsonObject obj = doc.object();

    if (!obj["nodes"].isArray()) {
        qWarning() << "no nodes in JSON-Object";
        return;
    }

    foreach (QJsonValue var, obj["nodes"].toArray()) {
        QString type, descr;
        QList<Gate> input_gates, output_gates;

        QJsonObject localNode = var.toObject();
        type = localNode["class"].toString();
        descr = localNode["description"].toString();
        QJsonArray inputs = localNode["input_gates"].toArray(),
                outputs = localNode["output_gates"].toArray();

        if (!inputs.isEmpty()) foreach (QJsonValue value, inputs) {
            QJsonObject localGate = value.toObject();
            QString gateName = localGate["name"].toString(),
                    gateType = localGate["type"].toString();
            Gate gate;
            gate.setDirection(true);
            gate.setName(gateName);
            gate.addType(gateType);
            input_gates << gate;
        }
        if (!outputs.isEmpty()) foreach (QJsonValue value, outputs) {
            QJsonObject localGate = value.toObject();
            QString gateName = localGate["name"].toString(),
                    gateType = localGate["type"].toString();
            Gate gate;
            gate.setDirection(false);
            gate.setName(gateName);
            gate.addType(gateType);
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
        /*input*/ const QJsonObject &obj, /*output*/ QList<Node> &nodelist,
        /*output*/ QList<Connection> &connectionlist) {
    // check if there are any nodes
    int i = 1, j = 1;  // for debug only
    if (!obj["nodes"].isArray()) {
        qWarning() << "no nodes in JSON-Object";
        return;
    }

    qDebug() << "File contains nodes, so let's parse them";

    QMap<QString, Node> nodemap;
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
            Node createdNode = NodeFactory::createNode(type, name, params);
            nodemap[createdNode.getName()] = createdNode;
        }
    }

    // nodes parsed
    nodelist = nodemap.values();

    if (!obj["connections"].isArray()) {
        qDebug() << "no connections found";
        return;
    }

    // TODO: SEGFAULT bei addGate in Node!!!
    foreach (QJsonValue var, obj["connections"].toArray()) {
        QVariantMap theConnection = var.toObject().toVariantMap();
        Node src_node = nodemap[theConnection["src_node"].toString()];
        Node dest_node = nodemap[theConnection["dest_node"].toString()];
        Connection connection(src_node, *src_node.getGateByName(theConnection["src_gate"].toString()), dest_node,  *dest_node.getGateByName(theConnection["dest_gate"].toString()));
        connectionlist << connection;
    }
}

// creates a new mesh and fills it with the Json content
/*Mesh JsonFileHandler::parseJsonString(QString &jsonString) {// <--- obsolete!
    // create QJsonObject from string given

    QJsonParseError *errorWhileParsing;
    QJsonDocument jsonDoc =
            QJsonDocument::fromJson(jsonString.toUtf8(), errorWhileParsing);
    QString errorMessage = errorWhileParsing->errorString();
    QJsonObject jsonObject = jsonDoc.object();
    qDebug() << "creating Json file:" << errorMessage;

    // create a new empty Mesh
    Mesh mesh = Mesh();
    // check if JSON-File is ok
    if (!(jsonObject.contains(QString("nodes")) && !jsonDoc.isEmpty() &&
          jsonObject["nodes"].isArray())) {
        QMessageBox::information(
                    0, QString("Error"),
                    QString("The file you selected has an unknown Format"),
"Ok");

        return mesh;
    }

    QJsonArray nodes;
    // create new mesh for returning it later
    nodes = jsonObject["nodes"].toArray();  // extract all nodes as Array
    foreach (QJsonValue node, nodes) {
        QJsonObject jNode = node.toObject();  // create a JSonObjec for each
node
        QString _class, _name;

        if (!jNode.contains("class"))  // check  if nodes are ok
        {
            QMessageBox::information(0, QString("Error"), QString("No Class
given"),
                                     "Ok");
            return mesh;
        } else if (!jNode.contains("name")) {
            QMessageBox::information(0, QString("Error"), QString("No Name
given"),
                                     "Ok");
            return mesh;

        } else if (!jNode.contains("params")) {
            QMessageBox::information(0, QString("Error"),
                                     QString("no parameters given"), "Ok");
            return mesh;

        } else if (!jNode["params"].isArray()) {
            QMessageBox::information(0, QString("Error"),
                                     QString("params is no array"), "Ok");
            return mesh;
        }

        _class = jNode["class"].toString();  // extract node type
        _name = jNode["name"].toString();    // extract node name

        QVariantMap _params = QVariantMap();  // set up params map

        QJsonArray jParams =
                jNode["params"].toArray();  // extract params as array from json
file
        // TODO: Talk about Gates ands Connections to get this working
        foreach (QJsonValue paramJValue, jParams) {  // for each of them..

            QJsonObject paramObject =
                    paramJValue.toObject(); //...make it an Object...

            foreach (QString paramKey, paramObject.keys()) {
                _params.insert(paramKey,
                               paramObject.find(paramKey)
                               .value()
                               .toString());  //...and push it into the map
            }
        }
        Node tmp = NodeFactory::createNode(
                    _class, _name,
                    _params);  // let the datafactory create a node and insert
it into mesh
        mesh.addNode(&tmp);
        qDebug() << "Node parsed:\n" << tmp.toString();
    }

    if (jsonObject.contains("connections")) {  // are there any connections?

        QJsonArray connections = jsonObject["connections"].toArray();
        if (connections.isEmpty())  // are there any connections?
            return mesh;
        foreach (QJsonValue localConnection, connections) {
            QJsonObject connectionObject = localConnection.toObject();

            // some error handling
            if (!(connectionObject.contains("src_node") &&
                  connectionObject.contains("src_gate") &&
                  connectionObject.contains("dest_node") &&
                  connectionObject.contains("dest_gate"))) {
                QString message = "";
                if (!connectionObject.contains("src_node")) {
                    message += "A connection has no src_node!\n";
                }
                if (!connectionObject.contains("src_gate")) {
                    message += "A connection has no src_gate!\n";
                }
                if (!connectionObject.contains("dest_node")) {
                    message += "A connection has no dest_node!\n";
                }
                if (!connectionObject.contains("dest_gate")) {
                    message += "A connection has no dest_gate!\n";
                }
                QMessageBox::information(0, QString("Error"), message, "Ok");
                return mesh;
            }  // error handling

            // get the Nodes
            Node *src_node =
                    mesh.getNodeByName(connectionObject["src_node"].toString()),
                    *dest_node =
                    mesh.getNodeByName(connectionObject["dest_node"].toString());
            // get the Gates
            Gate *src_gate =
                    src_node->getGateByName(connectionObject["src_gate"].toString()),
                    *dest_gate = dest_node->getGateByName(
                        connectionObject["dest_gate"].toString());

            // build up the Connection
            Connection *tmp =
                    new Connection(*src_node, *src_gate, *dest_node,
*dest_gate);

            mesh.addConnection(tmp);
        }
    }
    qDebug() << "created a mesh from the json file!";
    return mesh;
}*/

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
