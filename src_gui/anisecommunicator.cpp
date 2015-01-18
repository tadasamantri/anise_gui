#include "anisecommunicator.h"
#include <QDebug>


QProcess *AniseCommunicator::anise_process = new QProcess();
QString AniseCommunicator::path;
QString AniseCommunicator::readOutput;


//reads all available bytes from the Standard Output Channel
void AniseCommunicator::read(){

   //Sets the channel to be read to StandardOutput
   anise_process->setReadChannel(QProcess::StandardOutput);
   QByteArray ba = anise_process->readAll();
   readOutput = QString(ba);
}

//reads all available bytes from given Process Channel -- USE QProcess::... (see overloaded function read)
void AniseCommunicator::read(QProcess::ProcessChannel pc){

   //Sets the channel to be read
   anise_process->setReadChannel(pc);
   QByteArray ba = anise_process->readAll();
   readOutput = QString(ba);
}

//Returns Json String with all available Node Files
QString AniseCommunicator::getAllNodeTypes(){

   //create parameters needed for executing framework
   QStringList arguments;
   arguments << "--nodes" << "--machine";// << "--help";

   //get framework Path
   QString dir;
   dir += path;

   //remove name of Executable
   dir.chop(dir.length() - dir.lastIndexOf("/") - 1);


   //change working Directioy to get the Nodes =)
   AniseCommunicator::anise_process->setWorkingDirectory(dir);

   //execute framework
   AniseCommunicator::anise_process->start(path, arguments);

   //wait for prints
   anise_process->waitForFinished();

   //qDebug() << "finished";

   //read printed stuff
   read(QProcess::StandardError);

   qDebug() << readOutput;

   return "";
}


void AniseCommunicator::setFrameworkPath(QString newPath){

AniseCommunicator::path = newPath;

}
