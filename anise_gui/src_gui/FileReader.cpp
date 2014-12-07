#include "FileReader.h"


void FileReader::loadFile(QString path){

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));








    file = new QFile(path);
    if(file->open(QIODevice::ReadOnly)) {
        QJsonParseError e;
        QJsonDocument d = QJsonDocument::fromJson(file->readAll(), &e);
        if(!d.isNull()) {
            qDebug() << d.toJson(QJsonDocument::Compact);
        } else {
            qDebug() << e.errorString();
        }
    }














    //do something here





    file->close();
}


void FileReader::parseFile(){


}


void FileReader::writeFile(){


}
