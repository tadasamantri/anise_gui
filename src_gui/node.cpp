#include "node.h"


Node::Node(){
    inputGates = QVector<Gate>();
    outputGates = QVector<Gate>();
    params = QMap<QString, QString>();
    type = QString();
    name = QString();
}

Node::Node(QVector<Gate> &inputGates,
           QVector<Gate> &outputGates,
           QString &type,
           QString &name,
           QMap<QString, QString> &params)
{
    this->inputGates = inputGates;
    this->outputGates = outputGates;
    this->type = type;
    this->params = params;
    this->name = name;
}

Node::~Node(){
    delete &inputGates;
    delete &outputGates;
    delete &type;
    delete &params;
    delete &name;
}

void Node::setType(QString type){
    this->type = type;
}

void Node::setName(QString name){
    this->name = name;
}

QVector<Gate> Node::getInputGates(){
    return this->inputGates;
}

QVector<Gate> Node::getOutputGates(){
    return this->outputGates;
}

QString Node::getName(){
    return this->name;
}

QString Node::getType(){
    return this->type;
}

bool Node::addParam(QString _key, QString _value){
    if(!this->params.contains(_key))
        this->params.insert(_key,_value);
    else return false;
    return true;
}

bool Node::removeParam(QString _key){
    if(this->params.contains(_key))
        this->params.remove(_key);
    else return false;
    return true;
}

QString Node::getParamByKey(const QString &_key){
    if(!this->params.contains(_key))
        return QString("@@invalid@@");
    return params.find(_key).value();
}
