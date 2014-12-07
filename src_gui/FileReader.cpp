#include "FileReader.h"


void FileReader::loadFile(QString path){

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));


    file = new QFile(path);

    //do something here





    file->close();
}


void FileReader::parseFile(){


}


void FileReader::writeFile(){


}
