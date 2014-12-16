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


QJsonObject * JsonFileHandler::parseFile(QString &jsonString){
    //convert QString to char[]
    jsonString.remove(' ');
    jsonString.remove("\n");

    //create QJsonObject from string given
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jsonObject = jsonDoc.object();

    //check if JSON-File is ok
    if(jsonObject.contains(QString("nodes"))
            && jsonObject["nodes"].isArray()
            && jsonObject.contains(QString("connections"))
            && jsonObject["connections"].isArray()) {

        QJsonArray nodes, connections;
        nodes = jsonObject["nodes"].toArray();
        connections = jsonObject["connections"].toArray();
        qDebug() << "json parsed\n"<< jsonObject<<"\n\nnodes:\n" <<nodes<<"\n\nconnections:\n" << connections;
    } else {
       qDebug() << "file has a unknown format!";
    }
    return &jsonObject;
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
