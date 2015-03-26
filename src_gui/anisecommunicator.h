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
    * @bried readOuput always has the last output read
    */
    static QString readOutput;
    /**
    * @brief path to framework should be initialized while starting the gui
    */
    static QString path;
    /**
    * @brief Executes ANISE Framework
    */
    static QProcess *anise_process;
    static bool path_is_set;
    /**
     * @brief read reds specified channel
     * @param pc the channel to be read
     */
    static void read(QProcess::ProcessChannel pc);
    /**
     * @brief read reads all the output
     */
    static void read();
    /**
     * @brief validPath checks if given path is a valid framework exectuable
     * @param path
     * @return
     */
    static bool validPath(const QString &path);
    bool onProgress;
public:
    /**
     * @brief getAllNodeTypes executes command to get all existing node types from anise
     * @return the output given by anise
     */
    static QString getAllNodeTypes();
    /**
     * @brief setFrameworkPath store the framework path
     * @param path
     */
    static void setFrameworkPath(QString path);
    AniseCommunicator();
    ~AniseCommunicator();
    bool getOnProgress() const;
    void setOnProgress(bool value);
    /**
     * @brief runMesh starts the framework
     */
    void runMesh();
    void stop();
private slots:
    /**
     * @brief readProgress reads out progress information
     */
    void readProgress();
    /**
     * @brief finished called if anise finished;
     * @param exitCode
     */
    void finished(int exitCode);
};

#endif  // ANISECOMMUNICATOR_H
