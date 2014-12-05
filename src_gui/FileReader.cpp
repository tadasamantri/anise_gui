#include "FileReader.h"


void FileReader::loadFile(QString path){
    this->file.open(path);

    JsonText = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    this->file.close();
}


void FileReader::parseFile();


void FileReader::writeFile();
