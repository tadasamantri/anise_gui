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

    QJsonArray nodes, connections;
    //create new mesh for returning it later
    nodes = jsonObject["nodes"].toArray(); //extract all nodes as Array
    connections = jsonObject["connections"].toArray(); //same here with the connections
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
        mesh.addNode(tmp);
        qDebug() << "Node parsed:\n"<< tmp.toString();

        //TODO: Parse Connections!!!
    }
    //qDebug() << "json parsed\n"<< jsonObject<<"\n\nnodes:\n" <<nodes<<"\n\nconnections:\n" << connections;

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
