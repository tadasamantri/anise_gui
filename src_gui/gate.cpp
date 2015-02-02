#include "gate.h"
#include <QDebug>

Gate::Gate() {}

Gate::Gate(bool in, QList<QString> types) {
    this->in = in;
    this->types = types;
}

Gate::Gate(bool in, QString name) {
    this->in = in;
    this->name = name;
}

bool Gate::getDirection() { return this->in; }

bool Gate::hasType(const QString &type)
{
    return types.contains(type);
}

void Gate::addType(QString &type)
{
    if(!hasType(type))
        types << type;
}

bool Gate::removeType(const QString &type)
{
    if(hasType(type))
        types.removeAll(type);
    else return false;
    return true;
}

void Gate::setName(QString name){
    this->name = name;
}

void Gate::setDirection(bool in) { this->in = in; }

QList<QString> Gate::getTypes() { return types; }

QString Gate::getName() { return name; }
