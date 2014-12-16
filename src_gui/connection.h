#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include "node.h"
#include "gate.h"

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);
    Connection(Gate &src, Gate &dest);
    Gate* getSrcGate();
    Gate* getDestGate();
    void setSrcGate(Gate &src);
    void setDestGate(Gate &dest);
    //return true if validConnection(Node &src, &dest); Connection is made
    bool isValid();

private:
        Gate *gateA, *gateB;
signals:

public slots:

};

#endif // CONNECTION_H
