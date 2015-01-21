#include "jsonfilehandler.h"
#include <QJsonParseError>

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

// creates a new mesh and fills it with the Json content
Mesh JsonFileHandler::parseJsonString(QString &jsonString) {

    // create QJsonObject from string given

    QJsonParseError *errorWhileParsing;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(),errorWhileParsing);
    QString errorMessage = errorWhileParsing->errorString();
    QJsonObject jsonObject = jsonDoc.object();
    qDebug()<<"creating Json file:" << errorMessage;

    //create a new empty Mesh
    Mesh mesh = Mesh();
    // check if JSON-File is ok
    if (!(jsonObject.contains(QString("nodes")) && !jsonDoc.isEmpty() && jsonObject["nodes"].isArray())) {

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
        QJsonObject jNode = node.toObject();  // create a JSonObjec for each node
        QString _class, _name;

        if (!jNode.contains("class"))  // check  if nodes are ok
        {
            QMessageBox::information(0, QString("Error"), QString("No Class given"),
                                     "Ok");
            return mesh;
        } else if (!jNode.contains("name")) {
            QMessageBox::information(0, QString("Error"), QString("No Name given"),
                                     "Ok");
            return mesh;

        } else if (!jNode.contains("params")) {
            QMessageBox::information(0, QString("Error"),
                                     QString("no parameters given"),
                                     "Ok");
            return mesh;

        } else if (!jNode["params"].isArray()) {
            QMessageBox::information(0, QString("Error"),
                                     QString("params is no array"),
                                     "Ok");
            return mesh;
        }

        _class = jNode["class"].toString();  // extract node type
        _name = jNode["name"].toString();    // extract node name

        QVariantMap _params = QVariantMap();  // set up params map

        QJsonArray jParams =
                jNode["params"].toArray();  // extract params as array from json file
        // TODO: Talk about Gates ands Connections to get this working
        foreach (QJsonValue paramJValue, jParams) {  // for each of them..

            QJsonObject paramObject =
                    paramJValue.toObject();  //...make it an Object...

            foreach (QString paramKey, paramObject.keys()) {
                _params.insert(paramKey,
                               paramObject.find(paramKey)
                               .value()
                               .toString());  //...and push it into the map
            }
        }
        Node tmp = NodeFactory::createNode(
                    _class, _name,
                    _params);  // let the datafactory create a node and insert it into mesh
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
                QMessageBox::information(0, QString("Error"), message,
                                         "Ok");
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
                    new Connection(*src_node, *src_gate, *dest_node, *dest_gate);

            mesh.addConnection(tmp);
        }
    }
    return mesh;
}


void JsonFileHandler::printString(const QString &fileContent) {
    if (fileContent == "") {
        qDebug() << "no File Content loaded! \nthe parser didn't read the file "
                    "correctly!";
    } else {
        qDebug() << "File Content loaded!\n";
        qDebug() << "\n" << fileContent << "\n";
    }
}

void JsonFileHandler::writeFile(const QString &path,
                                const QString &fileContent) {
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << fileContent;
    file.close();
}

QString *JsonFileHandler::meshToJson(Mesh *mesh) {
    QString *jsonString = new QString();

    *jsonString += "{\n\t\"nodes: [";

    foreach (Node *localNode, mesh->getAllNodes()) {
        *jsonString += "\n\t\t{\"class\": " + localNode->getType() + "\",\n";
        *jsonString += "\t\t \"name\": \"" + localNode->getName() + "\", \n";
        *jsonString += "\t\t \"params\": [";
        foreach (QString key, localNode->params.keys()) {
            *jsonString += "\n\t\t\t{\"" + key + "\": \"" +
                    localNode->params[key].toString() + "\"},";
        }
        // remove obsolete last ","
        if (*(jsonString->end()) == ',') jsonString->chop(1);
        *jsonString += "]},";
    }
    // remove obsolete last ","
    if (*(jsonString->end()) == ',') jsonString->chop(1);
    *jsonString += "\t],\n";
    *jsonString += "\"connections\": [";

    foreach (Connection *localConnection, mesh->getAllConnections()) {
        Node *src = localConnection->getSrcNode(),
                *dst = localConnection->getDestNode();
        *jsonString += "\n\t{\"src_node\": \"";
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
    *jsonString += "\n\t]\n}";

    return jsonString;
}
