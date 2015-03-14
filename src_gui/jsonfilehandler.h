

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

/*
 * authors:
 * Tobi Lippert
 * Artur Fast
 * Mehrad Mohammadian
 *
 * Date:
 * 2.12.2014
 *
 */
#include <QString>
#include <QList>
#include <QJsonDocument>
#include "node.h"
#include "mesh.h"

class JsonFileHandler {
public:
    /*
     * will simply load the file
     */
    static QString loadFile(const QString &path);

    /*
     * Will extract information from the file
     */
    //static Mesh parseJsonString(QString &jsonString);  <-- obsolete!

    /*
     * will write the file to diskQString
     */

    static void saveMesh(const QString &path, Mesh *theMesh);

    /*
     * Print the File
     * (just for debugging)
     */

    static void printString(const QString &fileContent);

    static QString meshToJson(Mesh *mesh);

    /*
     * reads the given file and returns a QJsonObject
     */
    static QJsonObject *readFile(const QString &path);

    static void parseNodeTypesFromAnise(QString & output);

    /*
     * extracts all nodes given in QJsonObject and pushes them into the given list
     */
    static void extractNodesAndConnections(const QJsonObject &obj);
};

#endif  // FILEHANDLER_H
