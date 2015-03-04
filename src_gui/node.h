#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <QVariantMap>
#include "gate.h"

// Node konstruktor should be private
class Node {

    friend class JsonFileHandler;
    friend class NodeFactory;
    friend class NodeCatalog;


public:

    struct parameter{
        QString descr;
        QString key;
        QString name;
        QString type;
        QVariant value;
    };
    Node();
    float position_x;
    float position_y;
    QMap<QString, parameter>* getParams();

    Node(QVector<Gate*> &inputGates, QVector<Gate*> &outputGates, QString &type, QString &name, QMap<QString, parameter> &params);
    
    /*
   * TODO:
   * add gate ist sehr umständlich, am besten übergeben wir nur noch einen
   *zusätzlichen
   * type als(string, bool) und er erweitert seinen vector<gate> selber
   *
   */
    void addGate(Gate *gate);
    void addGates(QVector<Gate*> gates, const bool &direction);
    
    bool isInputGate(QString gateName);
    bool isOutputGate(QString gateName);

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

    QVector<Gate*> *getInputGates();

    QVector<Gate*> *getOutputGates();

    bool setParam(QString key, QVariant _value);

    bool removeParam(QString _key);

    QVariant getParamByKey(const QString &_key);

    QString toString();

    Gate *getGateByName(const QString &name);
    void addParam(QString key, parameter p);
    int x();
    int y();

    QString getDescription();
    void setDescription(const QString &value);
    void setX(const int &newX);
    void setY(const int &newY);
    int getID() const;
    void setID(int value);
    QPoint getPosition();
    QPoint getGatePosition(QString gateName);
    Node::parameter getParamStructByKey(const QString &key);
private:
    bool addParam(QString descr, QString _key, QString name, QString type, QVariant _value);
    QString description;
    QVector<Gate *> inputGates, outputGates;
    QString type, name;
    int ID = -2;
    QMap<QString, parameter> params;  // maps <identifier, value>
};

#endif  // NODE_H
