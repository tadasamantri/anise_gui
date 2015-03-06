#include "jsonfilehandler.h"
#include "data.h"
#include <QProcess>
#include <QJsonObject>
#include <QJsonArray>

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
        fileContent += line;
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

        QJsonObject localNode = var.toObject(), json_params;
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

        // initilize parameters
        json_params = localNode["parameters"].toObject();
        QJsonArray contents = localNode["parameters"].toArray();
        for (QJsonValue o : contents) {
            QVariantMap parameters = o.toObject().toVariantMap();
            node.addParam(
                        parameters["description"].toString(), parameters["key"].toString(),
                    parameters["name"].toString(), parameters["type"].toString(),
                    QVariant(
                        QVariant::nameToType(parameters["type"].toString().toUtf8())));
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

    qDebug() << "File contains " << obj["nodes"].toArray().size() << " nodes, so let's parse them ";
    Mesh *mesh = Data::instance()->getMesh();

    // for every node (represented as jsonvalue)...
    int counter = 0;
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
            Node *createdNode = NodeFactory::createNode(type);
            createdNode->setName(name);
            // get parameters as array of objects
            foreach (QJsonValue local, theNode["params"].toArray()) {
                // make every object a qvariantmap
                QVariantMap map = local.toObject().toVariantMap();
                // insert all records into params map

                foreach (QString key, map.keys()) {
                    createdNode->setParam(key, map[key]);
                    qDebug() << j++ << ": " << key << " = " << map[key];
                }
            }
            j = 1;  // for debugging only
            qDebug() << "\n";

            // parse the position and other gui_parameters
            if (theNode.contains("gui_params")) {
                QVariantMap p_gui = theNode["gui_params"].toObject().toVariantMap();
                createdNode->setPosition(p_gui["x"].toInt(), p_gui["y"].toInt());
            }
            // do some default stuff

            else {
                //Positions schätzung
                counter++;

                int offset = 100;

                int posx = 0;
                int posy = 0;

                //nodes with only outputs should be set left
                if(createdNode->getInputGates()->size() == 0){
                    posy += counter * offset;
                }else if(createdNode->getOutputGates()->size() == 0){
                    posx += offset * 12;
                    posy += counter * offset;
                }
                else{
                    posy += counter * offset;
                    posx += offset *(counter + createdNode->getOutputGates()->size() + createdNode->getInputGates()->size());
                }




                createdNode->setPosition(posx+20,posy+20);
            }
            // node is complete, so let's insert it

            mesh->addNode(createdNode);
        }
    }

    // nodes parsed
    if (!obj["connections"].isArray()) {
        qDebug() << "no connections found";
        return;
    }else{
        qDebug() << obj["connections"].toArray().size() <<" connections found";
    }

    foreach (QJsonValue var, obj["connections"].toArray()) {
        QJsonObject co = var.toObject();
        QVariantMap theConnection = co.toVariantMap();


        Node *src_node = mesh->getNodeByName(theConnection["src_node"].toString());
        Node *dest_node = mesh->getNodeByName(theConnection["dest_node"].toString());
        Connection *connection = new Connection(
                    src_node, src_node->getGateByName(theConnection["src_gate"].toString()),
                dest_node,
                dest_node->getGateByName(theConnection["dest_gate"].toString()));






        if (co.contains("gui_params")) {
            QJsonObject json_gui_params = co["gui_params"].toObject();
            QJsonArray way = json_gui_params["waypoints"].toArray();
            QVector<QPoint> waypoints;
            for (QJsonValue v : way) {
                QJsonObject o = v.toObject();
                if (o.contains("x") && o.contains("y"))
                    waypoints << QPoint(o["x"].toInt(), o["y"].toInt());
            }
            connection->setWaypoints(waypoints);
        }else{
            //if no gui params are found
            QVector<QPoint> waypoints;

            //start position of the connection
            //waypoints<< src_node->getGatePosition(theConnection["src_gate"].toString());

            //end position of the connections
            //waypoints<<  dest_node->getGatePosition(theConnection["dest_gate"].toString());


            //waypoints<< source << destination;


            connection->setWaypoints(waypoints);

        }

        qDebug() << "adding connection to Mesh";
        mesh->addConnection(connection);
    }
}

/**
 * @brief JsonFileHandler::writeFile writes a string into a given file
 * @param path path to file
 * @param fileContent the content written to the specified file
 */
void JsonFileHandler::saveMesh(const QString &path,Mesh *theMesh) {
    QFile file(path);

    file.open(QIODevice::WriteOnly);
    // QByteArray data = *JsonFileHandler::meshToJson(theMesh);
    file.write(meshToJson(theMesh).toUtf8());
    file.close();
}

QString JsonFileHandler::meshToJson(Mesh *mesh) {
    QJsonArray nodes, connections;
    foreach (Node *n, mesh->getAllNodes()) {
        QJsonObject theNode;
        theNode["class"] = n->getType();
        theNode["name"] = n->getName();
        QJsonArray params;
        QMap<QString, Node::parameter> *map = n->getParams();
        foreach (QString key, map->keys()) {
            QJsonObject param;
            QVariant var = map->value(key).value;
            param[key] = QJsonValue::fromVariant(var);
            params.push_back(param);
        }
        theNode["params"] = params;
        QJsonObject gui_params;
        gui_params["x"] = n->getPosition().x();
        gui_params["y"] = n->getPosition().y();
        theNode["gui_params"] = gui_params;
        nodes.push_back(theNode);
    }
    foreach (Connection *c, mesh->getAllConnections()) {
        QJsonObject theConnection;
        theConnection["src_node"] = c->getSrcNode()->getName();
        theConnection["src_gate"] = c->getSrcGate()->getName();
        theConnection["dest_node"] = c->getDestNode()->getName();
        theConnection["dest_gate"] = c->getDestGate()->getName();
        QJsonObject gui_params;
        QJsonArray way;
        QVector<QPoint> points = c->getWaypoints();
        for (int i = 0; i < points.size(); i++) {
            QPoint p = c->getWaypoints().at(i);
            QJsonObject point;
            point["x"] = p.x();
            point["y"] = p.y();
            way.push_back(point);
        }
        gui_params["waypoints"] = way;
        theConnection["gui_params"] = gui_params;
        connections.push_back(theConnection);
    }
    QJsonObject obj;
    obj.insert("nodes", nodes);
    obj.insert("connections", connections);
    QJsonDocument doc;
    doc.setObject(obj);
    return doc.toJson(QJsonDocument::Indented);
}
