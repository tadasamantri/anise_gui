#include "data.h"
#include "node.h"

Node::Node() {
  inputGates = QVector<Gate *>();
  outputGates = QVector<Gate *>();
  params = QVariantMap();
  type = QString();
  name = QString();
}

QVariantMap *Node::getParams() { return &this->params; }

Node::Node(QVector<Gate *> &inputGates, QVector<Gate *> &outputGates,
           QString &type, QString &name, QVariantMap &params) {
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

void Node::setName(QString name) {
  this->name = Data::instance()->getMesh()->getValidAlternativeForName(name);
}

QVector<Gate *> *Node::getInputGates() { return &this->inputGates; }

QVector<Gate *> *Node::getOutputGates() { return &this->outputGates; }

QString Node::getName() { return this->name; }

QString Node::getType() { return this->type; }

void Node::addGate(Gate *gate) {
  if (gate->getDirection() == true) {
    this->inputGates << gate;
  } else {
    this->outputGates << gate;
  }
}

void Node::addGates(QVector<Gate *> gates, const bool &direction) {
  if (direction == true) foreach (Gate *gate, gates) {
      inputGates << gate;
    }
  else
    foreach (Gate *gate, gates) { outputGates << gate; }
}

bool Node::addParam(QString _key, QVariant _value) {
  if (!this->params.contains(_key))
    this->params.insert(_key, _value);
  else
    return false;
  return true;
}

bool Node::setParam(QString key, QVariant _value) {
  if (params.contains(key)) {
    params[key] = _value;
    return true;
  }
  return false;
}

bool Node::removeParam(QString _key) {
  if (this->params.contains(_key))
    this->params.remove(_key);
  else
    return false;
  return true;
}

QVariant Node::getParamByKey(const QString &_key) {
  if (!this->params.contains(_key)) return QString("@@invalid@@");
  return params[_key];
}

QString Node::toString() {
  QString out = "";
  out += "class:" + type + "\nname:" + name + "\n\nparameters:\n";
  foreach (QString key, params.keys()) {
    out += "key:" + key + ", " + "value: " +
           params[key].typeName() + " " +
           params[key].toString() + "\n";
  }
  out += "position: (" + QString::number(position_x) + "|" +
         QString::number(position_y);
  return out;
}

Gate *Node::getGateByName(const QString &name) {
  foreach (Gate *gate, inputGates)
    if (gate->getName() == name) {
      return gate;
    }

  foreach (Gate *gate, outputGates)
    if (gate->getName() == name) {
      return gate;
    }
  return 0;
}

int Node::x() { return position_x; }

int Node::y() { return position_y; }

void Node::setX(const int &newX) { position_x = newX; }

void Node::setY(const int &newY) { position_y = newY; }

QString Node::getDescription() { return description; }

void Node::setDescription(const QString &value) { description = value; }
