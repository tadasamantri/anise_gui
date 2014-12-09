#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include "node.h"
class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);

    Node* nodeA, nodeB;

    //return true if valid Connection is made
    bool isValid();



signals:

public slots:

};

#endif // CONNECTION_H
