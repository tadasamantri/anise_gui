#ifndef ANISECOMMUNICATOR_H
#define ANISECOMMUNICATOR_H

#include <QString>
#include <QProcess>

/*
 * Author: Frederik Lührs (luehrs.fred@gmail.com)
 *
 * Execute ANISE framework. Mainly used to get console prints as a string
 *
 */

class AniseCommunicator : public QObject {

  Q_OBJECT

private:
  // readOutput always has the last output read
  static QString readOutput;
  // path to framework, should be initialized while starting gui
  static QString path;
  // Executes ANISE framework
  static QProcess *anise_process;
  static bool path_is_set;

  static void read(QProcess::ProcessChannel pc);
  static void read();

public:
  static QString getAllNodeTypes();
  static void setFrameworkPath(QString path);
};

#endif // ANISECOMMUNICATOR_H
