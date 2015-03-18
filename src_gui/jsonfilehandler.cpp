#include "jsonfilehandler.h"
#include "data.h"
#include <bitset>
#include <QProcess>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

QString JsonFileHandler::loadFile(const QString &path) {
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

    // catch error while parsing
    if (jerror.error != QJsonParseError::NoError) {
        qWarning() << jerror.errorString();
        QMessageBox::critical(Data::instance()->getMainWindow(),"Error while Parsing",jerror.errorString());
        return 0;
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
            QVariant value(QVariant::nameToType(parameters["type"].toString().toUtf8()));
            value = parameters["default"];
            node.addParam(
                        parameters["description"].toString(), parameters["key"].toString(),
                    parameters["name"].toString(), parameters["type"].toString(),value);
            node.setParam(parameters["name"].toString(),parameters["default"]);
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
    bool hasPositionData = true;
    //error flags: c|nc|n|nn
    //c: error in connection
    //nc: no connections
    //n: error in node
    //nn: no nodes
    std::bitset<4> flags = 0;
    // check if there are any nodes
    int i = 1, j = 1;  // for debug only
    if (!obj["nodes"].isArray()) {
        qWarning() << "no nodes in JSON-Object";
        flags |= 0b0001;
        goto end;
    }

    qDebug() << "File contains " << obj["nodes"].toArray().size()
             << " nodes";

    // for every node (represented as jsonvalue)...
    foreach (QJsonValue var, obj["nodes"].toArray()) {
        //...convert it to json object...
        QJsonObject theNode = var.toObject();

        // check if nodes are declared correctly
        if (!(theNode["class"].isString() && theNode["name"].isString() &&
              theNode["params"].isArray())){
            qWarning() << "Error while extracting node:\n" << theNode;
        flags |= 0b0010;
        }
        // node is welldefined:
        else {
            // get name and class(type) of node
            QString type = theNode["class"].toString(),
                    name = theNode["name"].toString();
            qDebug() << "node " << i++ << " parsed:\n name = " << name
                     << " | type = " << type;
            qDebug() << "params:";

            Node *createdNode = NodeFactory::createNode(type);
            //if node type not in catalog, skip
            if(createdNode->getType() == ""){
                flags |= 0b0010;
                continue;
            }
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
                createdNode->moveTo(p_gui["x"].toInt(), p_gui["y"].toInt());
            }
            else {
                hasPositionData = false;
            }
            // node is complete, so let's insert it

            Data::instance()->addNode(createdNode);
        }
    }

    // nodes parsed
    if (!obj["connections"].isArray()) {
        qDebug() << "no connections found";
        flags |= 0b0100;
        goto end;
    } else {
        qDebug() << obj["connections"].toArray().size() << " connections found";
    }

    foreach (QJsonValue var, obj["connections"].toArray()) {
        QJsonObject co = var.toObject();
        QVariantMap theConnection = co.toVariantMap();

        Node *src_node = Data::instance()->getNodeByName(theConnection["src_node"].toString());
        Node *dest_node =
                Data::instance()->getNodeByName(theConnection["dest_node"].toString());
        //if connection is invalid, skip
        if (!(src_node && dest_node) || !Data::instance()->checkConnection(src_node->getID(), theConnection["src_gate"].toString(),dest_node->getID(), theConnection["dest_gate"].toString())){
            flags |= 0b1000;
            continue;
        }

        Connection *connection = new Connection(
                    src_node, src_node->getGateByName(theConnection["src_gate"].toString()),
                dest_node,
                dest_node->getGateByName(theConnection["dest_gate"].toString()));

        if (co.contains("gui_params")) {
            QJsonObject json_gui_params = co["gui_params"].toObject();
            QJsonArray way = json_gui_params["waypoints"].toArray();
            QVector<QPoint> waypoints;
            for (QJsonValue v : way) {
                QJsonObject jWaypoints = v.toObject();
                if (jWaypoints.contains("x") && jWaypoints.contains("y"))
                    waypoints << QPoint(jWaypoints["x"].toInt(), jWaypoints["y"].toInt());
            }
            connection->setWaypoints(waypoints);
        } else {
            // if no gui params are found
            QVector<QPoint> waypoints;
            connection->setWaypoints(waypoints);
        }
        Data::instance()->addConnection(connection);
    }
    // all connections added"
    if (hasPositionData == false) {
        qDebug() << "position data missing";
        Data::instance()->sortForce();
    }
    end:  ;
    if(flags.any()){
        QString msg;
        msg += "Parser encountered Problems. File may be corrupt.\n\n \tIssues:\n";
        if(flags[0])
            msg += "\t- No Nodes were found\n";
        if(flags[1])
            msg += "\t- Error while parsing Node\n";
        if(flags[2])
            msg += "\t- No Connections founds\n";
        if(flags[3])
            msg += "\t- Error while parsing Connection";
        QMessageBox::warning(Data::instance()->getMainWindow(),"Errors while parsing!", msg);
    }
}

void JsonFileHandler::parseProgress(const QString &textline)
{

}

/**
 * @brief JsonFileHandler::writeFile writes a string into a given file
 * @param path path to file
 * @param fileContent the content written to the specified file
 */
void JsonFileHandler::saveMesh(const QString &path) {
    QFile file(path);

    file.open(QIODevice::WriteOnly);
    // QByteArray data = *JsonFileHandler::meshToJson(theMesh);
    file.write(meshToJson().toUtf8());
    file.close();
}

QString JsonFileHandler::meshToJson() {
    QJsonArray nodes, connections;
    foreach (Node *n, Data::instance()->getAllNodes()) {
        QJsonObject theNode;
        theNode["class"] = n->getType();
        theNode["name"] = n->getName();
        QJsonArray params;
        QMap<QString, Node::Parameter> *map = n->getParams();
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
    foreach (Connection *c, Data::instance()->getAllConnections()) {
        QJsonObject theConnection;
        theConnection["src_node"] = c->getSrcNode()->getName();
        theConnection["src_gate"] = c->getSrcGate()->getName();
        theConnection["dest_node"] = c->getDestNode()->getName();
        theConnection["dest_gate"] = c->getDestGate()->getName();
        QJsonObject gui_params;
        QJsonArray way;
        QVector<QPoint> points = *c->getWaypoints();
        for (int i = 0; i < points.size(); i++) {
            QPoint p = points.at(i);
            QJsonObject point;
            point["x"] = p.x();
            point["y"] = p.y();
            way<<(point);
        }
        gui_params["waypoints"] = way;
        theConnection["gui_params"] = gui_params;
        connections << theConnection;
    }
    QJsonObject obj;
    obj.insert("nodes", nodes);
    obj.insert("connections", connections);
    QJsonDocument doc;
    doc.setObject(obj);
    return doc.toJson(QJsonDocument::Indented);
}
