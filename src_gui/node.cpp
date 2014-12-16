#include "node.h"

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
