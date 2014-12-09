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

#include <QCoreApplication>
#include <QtDebug>

#include "qt-json/json.h"
#include "qjson4/QJsonDocument.h"


//test
#include "qjson4/QJsonValue.h"
#include "qjson4/QJsonDocument.h"
#include "qjson4/QJsonArray.h"
#include "qjson4/QJsonObject.h"
#include "qjson4/QJsonParseError.h"
#include <QFile>
#include <QByteArray>
#include <QCoreApplication>
#include <QTextCodec>
#include <QtDebug>



class FileReader{


private:

    QFile* file;
    const QString path;
    QStringList LineList;
    bool ok;


public:
    /*
     *will simply load the file
    */
    void loadFile(QString path);

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

    void printFile();
};
