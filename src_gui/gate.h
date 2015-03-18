#ifndef GATE_H
#define GATE_H

#include <QStringList>

class Gate {
public:
    Gate();
    Gate(const bool &in, const QStringList &types);
    Gate(const bool &in, const QString &name);
    bool getDirection() const;
    bool hasType(const QString &type) const;
    void addType(const QString &type);
    bool removeType(const QString &type);
    void setDirection(const bool &in);
    QStringList getTypes() const;
    QString getName() const;
    void setName(QString &name);
    QString getType() const;
private:
    QStringList types;
    bool in;
    QString name;
};

#endif  // GATE_H
