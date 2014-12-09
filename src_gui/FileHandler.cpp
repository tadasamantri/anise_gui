#include "FileHandler.h"


void FileHandler::loadFile(QString path){

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //open a file
    qDebug() << "trying to open: \n" << path << "\n";
    file = new QFile(path);
    file->open(QIODevice::ReadOnly);

    //read each line of that file and save it as a List
    //(line1)->(line2)->(line3)-> etc...
    while (file->bytesAvailable()) {

        QByteArray line = file->readLine();
        LineList << line;
    }

    file->close();
}


void FileHandler::parseFile(){


}

void FileHandler::printFile(){
    if (LineList.isEmpty()) {
        qDebug() << "LineList.isEmpty()! \nthe parser didnt read the file correctly!";
    }else{
        qDebug()<< "LineList is not Empty!:\n";
        foreach (QString line, LineList) {
            qDebug()<< "\n" << line;
        }
    }
}

void FileHandler::writeFile(){


}
