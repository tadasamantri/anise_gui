#ifndef GATE_H
#define GATE_H

#include <QStringList>

class Gate {
public:
    Gate();
    Gate(bool in, QList<QString> types);
    Gate(bool in, QString name);
    bool getDirection();
    bool hasType(const QString &type);
    void addType(QString &type);
    bool removeType(const QString &type);
    void setDirection(bool in);
    QList<QString> getTypes();
    QString getName();
    void setName(QString name);
    QString getType();
private:
    QList<QString> types;
    bool in;
    QString name;
    // Node* parent;
};

#endif  // GATE_H
