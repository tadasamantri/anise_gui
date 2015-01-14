#ifndef GATE_H
#define GATE_H

#include <QString>
#include<QVector>

class Gate
{

public:
    Gate();
    Gate(bool in, QVector<QString> tpyes);
    bool getDirection();
    void setDirection(bool in);
    QVector<QString> getTypes();
private:
    QVector<QString> types;
    bool in;



};

#endif // GATE_H
