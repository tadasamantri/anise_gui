

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
#include <QTextStream>

class FileHandler{

public:
    /*
     *will simply load the file
    */
    static QString loadFile(const QString &path);

    /*
     * Will extract information from the file
    */
    void parseFile();


    /*
    * will write the file to diskQString
    */

    void writeFile(const QString &path, const QString &fileContent);

    /*
     * Print the File
     * (just for debugging)
    */

    static void printFile(const QString &fileContent);
};

#endif //FILEHANDLER_H
