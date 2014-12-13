#ifndef ANISECOMMUNICATOR_H
#define ANISECOMMUNICATOR_H

#include <QString>
#include <QProcess>

class AniseCommunicator {

private:


    static QString path;
    static QString ANISE;
    static QProcess anise_process;
    static bool path_is_set;


public:

    static QString getAllNodeTypes();
    static void setFrameworkPath(QString path);


};


#endif // ANISECOMMUNICATOR_H
