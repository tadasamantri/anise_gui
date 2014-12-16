#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <QString>
#include <QMap>
#include "gate.h"

class Node
{
public:
    ~Node();
    Node();
    Node(QVector<Gate> &inputGates,
         QVector<Gate> &outputGates,
         QString &type,
         QString &name,
         QMap<QString,QString> &   params);

    int addGate(Gate &gate, int type); // type should be 0 for out, 1 for in, returns 0 on success
    void setType(QString type);
    QString getType();
    void setName(QString name);
    QString getName();
    QVector<Gate> getInputGates();
    QVector<Gate> getOutputGates();
    bool addParam(QString _key, QString _value);
    bool removeParam(QString _key);
    QString getParamByKey(const QString & _key);

private:
    QVector<Gate> inputGates, outputGates;
    QString type, name;
    QMap<QString,QString> params; //maps <identifier, value>


};

#endif // NODE_H
