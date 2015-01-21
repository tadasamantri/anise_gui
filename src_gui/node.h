#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <QString>
#include <QVariantMap>
#include <QStringBuilder>
#include "gate.h"

// Node konstruktor should be private
class Node {
public:
    Node();
    float position_x;
    float position_y;
    Node(QVector<Gate> &inputGates, QVector<Gate> &outputGates, QString &type,
         QString &name, QVariantMap &params);

    /*
   * TO-DO:
   * add gate ist sehr umständlich, am besten übergeben wir nur noch einen
   *zusätzlichen
   * type als(string, bool) und er erweitert seinen vector<gate> selber
   *
   */
    void addGate(Gate &gate, bool input);

    /*
   * Set the position of the Node
   * @param x new position_x
   * @param y new position_y
   */
    void setPosition(float x, float y);

    void setType(QString type);
    QString getType();
    void setName(QString name);
    QString getName();
    QVector<Gate> getInputGates();
    QVector<Gate> getOutputGates();
    bool addParam(QString _key, QVariant _value);
    bool removeParam(QString _key);
    QString getParamByKey(const QString &_key);
    QString toString();
    Gate *getGateByName(const QString &name);
    // private:
    QVector<Gate> inputGates, outputGates;
    QString type, name;
    QVariantMap params;  // maps <identifier, value>
};

#endif  // NODE_H
