#include "data.h"
#include "node.h"

Node::Node() {
    inputGates = QVector<Gate *>();
    outputGates = QVector<Gate *>();
    params = QMap<QString, parameter>();
    type = QString();
    name = QString();
}

QMap<QString, Node::parameter> *Node::getParams() { return &this->params; }

Node::Node(QVector<Gate *> &inputGates, QVector<Gate *> &outputGates,
           QString &type, QString &name, QMap<QString, parameter> &params) {
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
SingletonRender::instance()->setNodeName(ID, this->name);

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

bool Node::isInputGate(QString gateName)
{
    foreach(Gate *gate , *this->getInputGates()){
        
        if(gate->getName() == gateName)
            return gate->getDirection();
        
    }

    return false;
}

bool Node::isOutputGate(QString gateName)
{
    foreach(Gate *gate , *this->getOutputGates()){

        if(gate->getName() == gateName)
            return !gate->getDirection();

    }

    return false;


}

/**
 * @brief Node::addParam add a new Parameter to the Node
 * @param descr description
 * @param _key internal key
 * @param name displayed name
 * @param type data type
 * @param _value value to be stored
 * @return true if the parameter was added, false elsewise
 */
bool Node::addParam(QString descr, QString _key, QString name, QString type, QVariant _value) {
    if (!this->params.contains(_key)){
        parameter p{descr, _key, name, type, _value};
        this->params.insert(_key, p);
    }
    else
        return false;
    return true;
}

/**
 * @brief Node::setParam
 * @param key key of parameter to be changed
 * @param _value new value
 * @return true on success, false elsewise
 */
bool Node::setParam(QString key, QVariant _value) {
    if (params.contains(key)) {
        params[key].value = _value;
        return true;
    }
    return false;
}

/**
 * @brief Node::removeParam
 * @param _key key of parameter to remove
 * @return true on success
 */
bool Node::removeParam(QString _key) {
    if (this->params.contains(_key))
        this->params.remove(_key);
    else
        return false;
    return true;
}

Node::parameter Node::getParamStructByKey(const QString &key){
    if(params.contains(key))
        return params[key];
    else return parameter{QString(),QString(),QString(),QString(),QVariant()};
}

QVariant Node::getParamByKey(const QString &_key) {
    if (!this->params.contains(_key)) return QString("@@invalid@@");
    return params[_key].value;
}

QString Node::toString() {
    QString out = "";
    out += "class:" + type + "\nname:" + name + "\n\nparameters:\n";
    foreach (QString key, params.keys()) {
        out += "key:" + key + ", " + "value: " +
                params[key].value.typeName() + " " +
                params[key].value.toString() + "\n";
    }
    out += "position: (" + QString::number(position_x) + "|" +
            QString::number(position_y);
    return out;
}

Gate *Node::getGateByName(const QString &name) {

    foreach (Gate *gate, inputGates + outputGates)
        if (gate->getName() == name)
            return gate;

    return 0;
}

void Node::addParam(QString key, Node::parameter p)
{
    params[key] = p;
}

float Node::x() { return position_x; }

float Node::y() { return position_y; }

void Node::setX(const int &newX) { position_x = newX; }

void Node::setY(const int &newY) { position_y = newY; }

int Node::getID() const
{
    return ID;
}

void Node::setID(int value)
{
    ID = value;
}

QPoint Node::getPosition()
{
    return QPoint(this->position_x, this->position_y);
}

QPoint Node::getGatePosition(QString gateName){
    QPoint result;
    int gateHeight = SingletonRender::instance()->getImage("gate.png")->height();
    int gateWidth = SingletonRender::instance()->getImage("gate.png")->width();
    int drawObjectWidth = 3*16+2*gateWidth;
    int gateOffset = 10;
    int counter = 0;
    foreach (Gate *gate, outputGates){
        qDebug() << gate->getName();
        if (gate->getName()== gateName) {
            result = (QPoint(drawObjectWidth, counter * (gateHeight + gateOffset) + 5+gateHeight));
            result.operator +=(this->getPosition());
            qDebug() << result.x() << ";" << result.y();
            return result;
        }
        counter++;
    }
    counter = 0;
    foreach (Gate *gate, inputGates){
        if (gate->getName() == gateName) {

            result = (QPoint(0, counter * (gateHeight + gateOffset) + 5+gateHeight));
            result.operator +=(this->getPosition());
            return result;

        }
        counter++;
    }

    //best guess is the position of the node if no gate is found
    return this->getPosition();
}


QString Node::getDescription() { return description; }

void Node::setDescription(const QString &value) { description = value; }


