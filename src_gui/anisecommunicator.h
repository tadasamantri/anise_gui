#ifndef ANISECOMMUNICATOR_H
#define ANISECOMMUNICATOR_H

#include <QString>
#include <QProcess>

/**
 * @Author Frederik Lührs (luehrs.fred@gmail.com)
 *
 * Execute ANISE framework. Mainly used to get console prints as a string.
 *
 */

class AniseCommunicator : public QObject {

    Q_OBJECT

private:
    /**
      * readOuput always has the last output read
      */
    static QString readOutput;
    /**
      * path to framework should be initialized while starting the gui
      */
    static QString path;
    /**
      * Executes ANISE Framework
      */
    static QProcess *anise_process;
    static bool path_is_set;

    static void read(QProcess::ProcessChannel pc);
    static void read();
    static bool validPath(const QString &path);

public:
    static QString getAllNodeTypes();
    static void setFrameworkPath(QString path);
};

#endif // ANISECOMMUNICATOR_H
