#ifndef DATA_H
#define DATA_H

#include <QObject>

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = 0);
    Mesh mesh;
    NodeCatalog nodeCatalog;


signals:

public slots:


};

#endif // DATA_H
