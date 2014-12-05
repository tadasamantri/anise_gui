/*
 * authors:
 * Artur Fast
 * Mehrad Mohammadian
 *
 * Date:
 * 2.12.2014
 *
 */

#include "qt-json/json.h"
#include "qjson4/QJsonDocument.h"
#include <QFile>
#include <QString>

class FileReader{

private:
    QJsonDocument JsonText;
    QFile file;


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
};
