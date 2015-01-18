#include "jsonfilehandler.h"


QString JsonFileHandler::loadFile(const QString &path){

    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QString fileContent = "";


    //open a file
    qDebug() << "trying to open: \n" << path << "\n";
    QFile file(path);
    file.open(QIODevice::ReadOnly);


    //read each line of that file and append it to the String
    while (file.bytesAvailable()) {

        QByteArray line = file.readLine();
        fileContent.append(line);
    }

    file.close();
    return fileContent;
}


Mesh JsonFileHandler::parseJsonString(QString &jsonString){
    //convert QString to char[]
    jsonString.remove(' ');
    jsonString.remove("\n");

    //create QJsonObject from string given
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jsonObject = jsonDoc.object();
    Mesh mesh = Mesh();
    //check if JSON-File is ok
    if(!(jsonObject.contains(QString("nodes"))
         && jsonObject["nodes"].isArray())){
        // && jsonObject.contains(QString("connections"))
        //&& jsonObject["connections"].isArray())) {

        QMessageBox::information(0,QString("Error"),QString("The file you selected has an unknown Format"), "Thanks for that!");
        return mesh;
    }

    QJsonArray nodes;
    //create new mesh for returning it later
    nodes = jsonObject["nodes"].toArray(); //extract all nodes as Array
    foreach (QJsonValue node, nodes) {

        QJsonObject jNode = node.toObject(); //create a JSonObjec for each node
        QString _class, _name;

        if(!jNode.contains("class")) //check  if nodes are ok
        {
            QMessageBox::information(0,QString("Error"),QString("No Class given"), "Thanks for that!");
            return mesh;
        } else if (!jNode.contains("name")){
            QMessageBox::information(0,QString("Error"),QString("No Name given"), "Thanks for that!");
            return mesh;

        }else if (!jNode.contains("params")){
            QMessageBox::information(0,QString("Error"),QString("no parameters given"), "Thanks for that!");
            return mesh;

        }else if (!jNode["params"].isArray()){
            QMessageBox::information(0,QString("Error"),QString("params is no array"), "Thanks for that!");
            return mesh;

        }

        _class = jNode["class"].toString(); //extract node type
        _name = jNode["name"].toString(); //extract node name

        QVariantMap _params = QVariantMap(); //set up params map

        QJsonArray jParams = jNode["params"].toArray(); //extract params as array from json file

        foreach (QJsonValue paramJValue, jParams) { //for each of them..

            QJsonObject paramObject = paramJValue.toObject(); //...make it an Object...

            foreach(QString paramKey, paramObject.keys()){
                _params.insert(paramKey, paramObject.find(paramKey).value().toString()); //...and push it into the map
            }
        }
        Node tmp = NodeFactory::createNode(_class, _name, _params); //let the datafactory create a node and insert it into mesh
        mesh.addNode(&tmp);
        qDebug() << "Node parsed:\n"<< tmp.toString();
    }

    if(jsonObject.contains("connections")){ //are there any connections?

        QJsonArray connections = jsonObject["connections"].toArray();
        if(connections.isEmpty()) //are there any connections?
            return mesh;
        foreach (QJsonValue vConnection, connections) {
            QJsonObject object = vConnection.toObject();

            //some error handling
            if(!(object.contains("src_node")
                 && object.contains("src_gate")
                 && object.contains("dest_node")
                 && object.contains("dest_gate"))){
                QString message = "";
                if(!object.contains("src_node")){
                    message += "A connection has no src_node!\n";
                }
                if(!object.contains("src_gate")){
                    message += "A connection has no src_gate!\n";
                }
                if(!object.contains("dest_node")){
                    message += "A connection has no dest_node!\n";
                }
                if(!object.contains("dest_gate")){
                    message += "A connection has no dest_gate!\n";
                }
                QMessageBox::information(0,QString("Error"),message, "Thanks for that!");
                return mesh;
            }//error handling

            //TODO: Talk about Gates ands Connections to get this working
        }

    }
    return mesh;
}

void JsonFileHandler::printFile(const QString &fileContent){
    if (fileContent == "" ) {
        qDebug() << "no File Content loaded! \nthe parser didn't read the file correctly!";
    }else{
        qDebug()<< "File Content loaded!:\n";
        qDebug()<< "\n" << fileContent << "\n";

    }
}

void JsonFileHandler::writeFile(const QString &path, const QString &fileContent){

    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << fileContent;
    file.close();

}

QString* JsonFileHandler::meshToJson(Mesh *mesh){

    QString *jsonString = new QString();

    *jsonString += "{\n\t\"nodes: [";

    foreach (Node* localNode, mesh->nodes) {
        *jsonString += "\n\t\t{\"class\": " + localNode->getType() + "\",\n";
        *jsonString += "\t\t \"name\": \"" + localNode->getName() + "\", \n";
        *jsonString += "\t\t \"params\": [";
        foreach(QString key, localNode->params.keys()){
            *jsonString += "\n\t\t\t{\"" + key + "\": \"" + localNode->params[key].toString() + "\"},";
        }
        //remove obsolete last ","
        if(*(jsonString->end()) == ',')
            jsonString->chop(jsonString->length()-1);
        *jsonString += "]},";

    }
    //remove obsolete last ","
    if(*(jsonString->end()) == ',')
        jsonString->chop(jsonString->length()-1);
    *jsonString += "\t],\n";
    *jsonString += "\"connections\": [";

    foreach (Connection* localConnection, mesh->connections) {
        *jsonString += "\n\t{\"src_node\": \"";
        *jsonString += /*TODO: NodeName*/ "NodeNameDummy";
        *jsonString += "\", \"src_gate\": \"" ;
        *jsonString += /*TODO:GateName*/ "GateNameDummy (out)";
        *jsonString += "\", ";
        *jsonString += "\"dest_node\": \"";
        *jsonString += /*TODO: NodeName*/ "NodeNameDummy";
        *jsonString += "\", \"dest_gate\": \"";
        *jsonString += /*T + ODO:GateName*/ "GateNameDummy (in)";
        *jsonString += "\"},";
    }
    //remove obsolete last ","
    if(*(jsonString->end()) == ',')
        jsonString->chop(jsonString->length()-1);
    *jsonString += "\n\t]\n}";

    return jsonString;

}
