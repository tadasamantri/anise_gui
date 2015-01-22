#ifndef GATE_H
#define GATE_H

#include <QString>
#include <QVector>

class Gate {
public:
    Gate();
    Gate(bool in, QVector<QString> tpyes);
    Gate(bool in, QString name);
    bool getDirection();
    void setDirection(bool in);
    QVector<QString> getTypes();
    static const bool DIRECTION_IN = true;
    static const bool DIRECTION_OUT = false;
    QString getName();
    void setName(QString name);

private:
    QVector<QString> types;
    bool in;
    QString name;
    // Node* parent;
};

#endif  // GATE_H
