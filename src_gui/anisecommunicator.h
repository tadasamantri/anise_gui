#ifndef ANISECOMMUNICATOR_H
#define ANISECOMMUNICATOR_H

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
    bool onProgress;

public:
    static QString getAllNodeTypes();
    static void setFrameworkPath(QString path);
    ~AniseCommunicator();
    bool getOnProgress() const;
    void setOnProgress(bool value);

    void runMesh();
private slots:
    void readProgress();
};

#endif  // ANISECOMMUNICATOR_H
