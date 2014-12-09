

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

/*
 * authors:
 * Artur Fast
 * Mehrad Mohammadian
 *
 * Date:
 * 2.12.2014
 *
 */


#include <QFile>
#include <QString>
#include <QTextCodec>
#include <QtDebug>
#include "qjson4/QJsonObject.h"


class FileHandler{

public:
    /*
     *will simply load the file
    */
    static QString loadFile(QString &path);

    /*
     * Will extract information from the file
    */
    void parseFile();


    /*
    * will write the file to disk
    */

    void writeFile();

    /*
     * Print the File
     * (just for debugging)
    */

    static void printFile(QString &fileContent);
};

#endif //FILEHANDLER_H
