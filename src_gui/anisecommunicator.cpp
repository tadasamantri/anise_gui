#include "anisecommunicator.h"
#include <QDebug>

QProcess *AniseCommunicator::anise_process = new QProcess();
QString AniseCommunicator::path;
QString AniseCommunicator::readOutput;

void AniseCommunicator::readAll(){

   QByteArray ba = anise_process->readAll();
   readOutput = QString(ba);
}

QString AniseCommunicator::getAllNodeTypes(){

    QStringList arguments;
    arguments << "--nodes" << "--machine";
    qDebug() << path << arguments;

    //QObject::connect(anise_process, &QProcess::finished, readAll);
    AniseCommunicator::anise_process->start(path, arguments);
    anise_process->setReadChannel(QProcess::StandardOutput);

    anise_process->waitForFinished();
    qDebug() << "finished";

    readAll();

    QByteArray byteArrayOutput = anise_process->readAllStandardOutput();

    QString output(byteArrayOutput);



    qDebug() << output;
    //if(byteArrayOutput.isEmpty()){
   // qDebug() << byteArrayOutput;
    return "";
}



void AniseCommunicator::setFrameworkPath(QString newPath){

AniseCommunicator::path = newPath;

}
