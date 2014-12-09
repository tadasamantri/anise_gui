#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QVector>
#include <QString>

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = 0);

struct Gate{
    QString type;
};

QVector<Gate> gates;
signals:

public slots:

};

#endif // NODE_H
