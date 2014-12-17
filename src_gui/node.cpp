#include "node.h"


Node::Node(){
    inputGates = QVector<Gate>();
    outputGates = QVector<Gate>();
    params = QVariantMap();
    type = QString();
    name = QString();
}

Node::Node(QVector<Gate> &inputGates,
           QVector<Gate> &outputGates,
           QString &type,
           QString &name,
           QVariantMap &params)
{
    this->inputGates = inputGates;
    this->outputGates = outputGates;
    this->type = type;
    this->params = params;
    this->name = name;
}

Node::~Node(){

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

bool Node::addParam(QString _key, QVariant _value){
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
    return params.find(_key).value().toString();
}

QString Node::toString(){
    QString out = "";
    out = out.append("class:").append(type).append("\nname:").append(name).append("\n\nparameters:\n");
    foreach(QString key, params.keys()){
        out = out.append("key:").append(key).append(", ").append("value:").append(params.find(key).value().toString()).append("\n");
    }
    return out;
}
