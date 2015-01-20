

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
#include <QErrorMessage>
#include <QMessageBox>
#include "node.h"
#include "nodefactory.h"
#include "mesh.h"

class JsonFileHandler {

public:
  /*
   *will simply load the file
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

  static void printFile(const QString &fileContent);

  static QString *meshToJson(Mesh *mesh);
};

#endif // FILEHANDLER_H
