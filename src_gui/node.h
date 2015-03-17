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
    struct Parameter {
        QString descr;
        QString key;
        QString name;
        QString type;
        QVariant value;
    };

    enum Status {
        idle,
        busy,
        finished,
    };

    Node();
    float position_x;
    float position_y;
    QMap<QString, Parameter> *getParams();

    Node(QVector<Gate *> &inputGates, QVector<Gate *> &outputGates, QString &type,
         QString &name, QMap<QString, Parameter> &params);

    /*
 * TODO:
 * add gate ist sehr umständlich, am besten übergeben wir nur noch einen
 *zusätzlichen
 * type als(string, bool) und er erweitert seinen vector<gate> selber
 *
 */
    void addGate(Gate *gate);
    void addGates(QVector<Gate *> gates, const bool &direction);

    bool isInputGate(QString gateName);
    bool isOutputGate(QString gateName);

    /*
 * Set the position of the Node
 * @param x new position_x
 * @param y new position_y
 */
    void moveTo(float x, float y);

    void setType(QString type);

    QString getType();

    void setName(QString name);

    QString getName();

    QVector<Gate *> *getInputGates();

    QVector<Gate *> *getOutputGates();

    bool setParam(QString key, QVariant _value);

    bool removeParam(QString _key);

    QVariant getParamByKey(const QString &_key);

    QString toString();

    Gate *getGateByName(const QString &name);
    void addParam(QString key, Parameter p);
    float x();
    float y();

    QString getDescription();
    void setDescription(const QString &value);
    void setX(const int &newX);
    void setY(const int &newY);
    int getID() const;
    void setID(int value);
    QPoint getPosition();
    QPoint getGatePosition(QString gateName);
    Node::Parameter getParamStructByKey(const QString &key);
    float getProgress() const;
    void setProgress(float value);

private:
    bool addParam(QString descr, QString _key, QString name, QString type,
                  QVariant _value);
    QString description;
    QVector<Gate *> inputGates, outputGates;
    QString type, name;
    int ID = -2;
    QMap<QString, Parameter> params;  // maps <identifier, value>
    float progress;
};

#endif  // NODE_H
