#include "anisecommunicator.h"
#include "jsonfilehandler.h"
#include <QDebug>
#include <QFileInfo>
#include "data.h"

QProcess *AniseCommunicator::anise_process = new QProcess();
QString AniseCommunicator::path;
QString AniseCommunicator::readOutput;

/**
  * AniseCommunicator read all available bytes from the Standard output Channel
  *
  */
void AniseCommunicator::read() {
    /*
  * Sets the channel to be read to StandardOutput
  */
    anise_process->setReadChannel(QProcess::StandardOutput);
    QByteArray ba = anise_process->readAll();
    readOutput = QString(ba);
}

/**
 * @brief AniseCommunicator::validPath
 * @param path
 * @return true if path leads to anise executable
 */
bool AniseCommunicator::validPath(const QString &path) {
    QProcess checker;
    QStringList arg;
    arg << "--version";
    //ask anise for version output
    checker.start(path, arg);
    // wait up to 3s for an answer
    checker.waitForFinished(3000);
    QString result = checker.readAll();
    //check if
    return result.contains("anise-framework");
}

void AniseCommunicator::stop() { anise_process->terminate(); }

bool AniseCommunicator::getOnProgress() const { return onProgress; }

void AniseCommunicator::setOnProgress(bool value) { onProgress = value; }

/**
  * AniseCommunicator read all available bytes from given Process Channel
  * It use QProcess.
  * Changes the readOutput.
  * @see    Overloaded function read
  */
void AniseCommunicator::read(QProcess::ProcessChannel pc) {
    /*
  * Sets the channel to be read
  */
    anise_process->setReadChannel(pc);
    QByteArray ba = anise_process->readAll();
    readOutput = QString(ba);
}

/*
  * Returns a Json string with all available Node files.
  * This method always returns the existing node types in the
  * Json string.
  *
  */
QString AniseCommunicator::getAllNodeTypes() {
    // Created parameters for executing the Framework:
    QStringList arguments;
    arguments << "--nodes"
              << "--machine";

    /*
  * Starts the Framework.
  */
    anise_process->start(path, arguments);

    /*
  * Waits for prints.
  */
    anise_process->waitForFinished();
    /*
  * Read printed stuff.
  */
    read();
    //cut off unwanted characters like " or whitespace
    readOutput =
            readOutput.mid(readOutput.indexOf("{"), readOutput.lastIndexOf("}") + 1);

    anise_process->close();
    return readOutput;
}

void AniseCommunicator::setFrameworkPath(QString newPath) {
    AniseCommunicator::path = newPath;

    QFileInfo f(newPath);
    anise_process->setWorkingDirectory(f.absolutePath());
}

AniseCommunicator::AniseCommunicator() {
    onProgress = false;
    connect(anise_process, SIGNAL(readyRead()), this, SLOT(readProgress()));
    connect(anise_process, SIGNAL(finished(int)), this, SLOT(finished(int)));
}

AniseCommunicator::~AniseCommunicator() {
    disconnect(anise_process, SIGNAL(readyRead()), this, SLOT(readProgress()));
    disconnect(anise_process, SIGNAL(finished(int)), this, SLOT(finished(int)));
    delete anise_process;
}

void AniseCommunicator::readProgress() {
    if (!onProgress) return;
    readOutput = (QString)anise_process->readAllStandardOutput();
    QStringList lines = readOutput.split("\n");
    for (QString line : lines)
        JsonFileHandler::parseProgress(line, JsonFileHandler::progress);
    readOutput = (QString)anise_process->readAllStandardError();
    lines = readOutput.split("\n");
    for (QString line : lines)
        JsonFileHandler::parseProgress(line, JsonFileHandler::error);
}

void AniseCommunicator::finished(int exitCode) {
    // TODO: exit code handling!
    onProgress = false;
    Data::instance()->finishMesh();
    qDebug() << "Anise finished with exit code" << exitCode;
}

void AniseCommunicator::runMesh() {
    QStringList args;
    // use arguments --progress and --machine
    args << Data::instance()->getSaveFile() << "--machine"
         << "--progress";
    //set the correct working directory
    anise_process->setWorkingDirectory(QFileInfo(path).absolutePath());
    onProgress = true;
    //execute anise
    anise_process->start(path, args);
}
