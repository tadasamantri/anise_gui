#include "FileReader.h"


void FileReader::loadFile(Qstring path){
    this->file.open(path);

    JsonText = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    this->file.close();
}


void File::parseFile();


void File::writeFile();
