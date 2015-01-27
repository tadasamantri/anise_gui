#include "anisecommunicator.h"
#include <QDebug>

QProcess *AniseCommunicator::anise_process = new QProcess();
QString AniseCommunicator::path;
QString AniseCommunicator::readOutput;


/**
  * AniseCommunicator read all available bytes from the Standard output Channel
  *
  */
void AniseCommunicator::read() {
    /**
      * Sets the channel to be read to StandardOutput
      */
    anise_process->setReadChannel(QProcess::StandardOutput);
    QByteArray ba = anise_process->readAll();
    readOutput = QString(ba);
}

/**
  * AniseCommunicator read all available bytes from given Process Channel
  * It use QProcess.
  * Changes the readOutput.
  * @see    Overloaded function read
  */
void AniseCommunicator::read(QProcess::ProcessChannel pc) {
    /**
      * Sets the channel to be read
      */
    anise_process->setReadChannel(pc);
    QByteArray ba = anise_process->readAll();
    readOutput = QString(ba);
}

/**
  * Returns a Json string with all available Node files.
  * This method always returns the existing node types in the
  * Json string.
  *
  */
QString AniseCommunicator::getAllNodeTypes() {
    /**
      * Created parameters for executing the Framework:
      */
    QStringList arguments;
    arguments << "--nodes"
              << "--machine";

    /**
      * QString dir has the framework path.
      */
    QString dir;
    dir += path;

    /**
      * Removes name of the Executable.
      * dir is now path of directory containing anise.
      * Example: from "~/bla/bla/anise" to "~/bla/bla/"
      */
    dir.chop(dir.length() - dir.lastIndexOf("/") - 1);

    /**
      * Changes the working Directory to get the Nodes.
      */
    anise_process->setWorkingDirectory(dir);

    /**
      * Starts the Framework.
      */
    anise_process->start(path, arguments);

    /**
      * Waits for prints.
      */
    anise_process->waitForFinished();

    /**
      * Read printed stuff.
      */
    read();
    readOutput = readOutput.mid(readOutput.indexOf("{"),readOutput.lastIndexOf("}") - 1);
    qDebug() << "this is what the communicator got from the framework:\n"<<readOutput <<"\n";
    return readOutput;
}

void AniseCommunicator::setFrameworkPath(QString newPath) {
    AniseCommunicator::path = newPath;
}
