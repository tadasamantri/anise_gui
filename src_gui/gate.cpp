#include "gate.h"

Gate::Gate() {
    types = QStringList();
}

Gate::Gate(bool const &in, const QStringList &types) {
    this->in = in;
    this->types = types;
}

Gate::Gate(bool const &in, QString const &name) {
    this->in = in;
    this->name = name;
}

bool Gate::getDirection() const { return this->in; }

bool Gate::hasType(const QString &type) const { return types.contains(type); }

void Gate::addType(const QString &type) {
    if (!hasType(type)) types << type;
}

bool Gate::removeType(const QString &type) {
    if (hasType(type))
        types.removeAll(type);
    else
        return false;
    return true;
}

void Gate::setName(QString &name) { this->name = name; }

void Gate::setDirection(const bool &in) { this->in = in; }

QStringList Gate::getTypes() const { return types; }

QString Gate::getType() const { return types.first(); }

QString Gate::getName() const { return name; }
