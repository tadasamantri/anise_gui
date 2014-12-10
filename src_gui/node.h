#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMap>

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = 0);
    ~Node();
    struct Gate{
        QString type;
    };
Node(QVector<Gate> &inputGates,
     QVector<Gate> &outputGates,
     QString &type,
     QString &name,
     QMap<QString,QString> &   params);

int addGate(Gate &gate, int type); // type should be 0 for out, 1 for in, returns 0 on success




private:
QVector<Gate> inputGates, outputGates;
QString type, name;
QMap<QString,QString> params; //maps <identifier, value>

signals:

public slots:

};

#endif // NODE_H
