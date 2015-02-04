#include "node.h"

Node::Node() {
    inputGates = QVector<Gate*>();
    outputGates = QVector<Gate*>();
    params = QVariantMap();
    type = QString();
    name = QString();
}

QVariantMap *Node::getParams()
{
    return &this->params;
}

Node::Node(QVector<Gate*> &inputGates, QVector<Gate*> &outputGates, QString &type,
           QString &name, QVariantMap &params) {
    this->inputGates = inputGates;
    this->outputGates = outputGates;
    this->type = type;
    this->params = params;
    this->name = name;
}

void Node::setPosition(float x, float y) {
    this->position_x = x;
    this->position_y = y;
}

void Node::setType(QString type) { this->type = type; }

void Node::setName(QString name) { this->name = name; }

QVector<Gate *> *Node::getInputGates() { return &this->inputGates; }

QVector<Gate *> *Node::getOutputGates() { return &this->outputGates; }

QString Node::getName() { return this->name; }

QString Node::getType() { return this->type; }

void Node::addGate(Gate *gate){
    if (gate->getDirection() == true) {
        this->inputGates << gate;
    } else {
        this->outputGates << gate;
    }
}

void Node::addGates(QVector<Gate*> gates, const bool &direction)
{
    if(direction == true)
        foreach (Gate *gate, gates) {
            inputGates.append(gate);
        }
    else
        foreach (Gate *gate, gates) {
            outputGates.append(gate);
        }
}

bool Node::addParam(QString _key, QVariant _value) {
    if (!this->params.contains(_key))
        this->params.insert(_key, _value);
    else
        return false;
    return true;
}

bool Node::removeParam(QString _key) {
    if (this->params.contains(_key))
        this->params.remove(_key);
    else
        return false;
    return true;
}

QString Node::getParamByKey(const QString &_key) {
    if (!this->params.contains(_key)) return QString("@@invalid@@");
    return params.find(_key).value().toString();
}

QString Node::toString() {
    QString out = "";
    out = out.append("class:").append(type).append("\nname:").append(name).append(
                "\n\nparameters:\n");
    foreach (QString key, params.keys()) {
        out = out.append("key:")
                .append(key)
                .append(", ")
                .append("value: ")
                .append(params.find(key).value().typeName())
                .append(" ")
                .append(params.find(key).value().toString())
                .append("\n");
    }
    return out;
}

Gate *Node::getGateByName(const QString &name) {
    Gate *g = 0;
    foreach (Gate *gate, this->inputGates)
        if (gate->getName() == name) {
            g = gate;
            return g;
        }

    foreach (Gate* gate, this->outputGates)
        if (gate->getName() == name) {
            g = gate;
            break;
        }

    return g;
}
QString Node::getDescription()
{
    return description;
}

void Node::setDescription(const QString &value)
{
    description = value;
}

