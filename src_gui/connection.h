#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include "node.h"
class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);
    ~Connection();
    Connection(Node &src, Node &dest);
    Node* getSrcNode();
    Node* getDestNode();
    void setSrcNode(Node &src);
    void setDestNode(Node &dest);
    //return true if validConnection(Node &src, &dest); Connection is made
    bool isValid();

private:
        Node *nodeA, *nodeB;
signals:

public slots:

};

#endif // CONNECTION_H
