

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

#include <QFile>
#include <QString>
#include <QVector>
#include <QTextCodec>
#include <QtDebug>
#include <QJsonObject>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonParseError>
#include <QErrorMessage>
#include <QMessageBox>
#include "node.h"
#include "nodefactory.h"
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
    static Mesh parseJsonString(QString &jsonString);

    /*
     * will write the file to diskQString
     */

    void writeFile(const QString &path, const QString &fileContent);

    /*
     * Print the File
     * (just for debugging)
     */

    static void printString(const QString &fileContent);

    static QString *meshToJson(Mesh *mesh);

    /*
     * reads the given file and returns a QJsonObject
     */
    static QJsonObject *readFile(const QString &path);

    /*
     * extracts all nodes given in QJsonObject and pushes them into the given list
     */
    static void extractNodesAndConnections(const QJsonObject &obj, QList<Node*> &nodelist, QList<Connection*> &connectionlist);
};

#endif  // FILEHANDLER_H
