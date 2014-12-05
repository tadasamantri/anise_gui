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


class FileReader{

private:
    QJasonDocument JsonText;
    QFile file;


public:
    /*
     *will simply load the file
    */
    void loadFile(Qstring path);

    /*
     * Will extract information from the file
    */
    void parseFile();


    /*
    * will write the file to disk
    */

    void writeFile();
};
