#include "gate.h"

Gate::Gate(bool in, QVector<QString> types){
    this->in=in;
    this->types=types;
}

bool Gate::getDirection(){
    return this->in;
}

void Gate::setDirection(bool in){
    this->in = in;
}


QVector<QString> Gate::getTypes(){
    return types;
}

