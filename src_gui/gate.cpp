#include "gate.h"

Gate::Gate() {}

Gate::Gate(bool in, QVector<QString> types) {
    this->in = in;
    this->types = types;
}

Gate::Gate(bool in, QString name) {
    this->in = in;
    this->name = name;
}

bool Gate::getDirection() { return this->in; }

void Gate::setDirection(bool in) { this->in = in; }

QVector<QString> Gate::getTypes() { return types; }

QString Gate::getName() { return name; }
