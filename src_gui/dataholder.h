#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <QObject>
#include "data.h"

class DataHolder : public QObject
{
    Q_OBJECT
public:
    explicit DataHolder(QObject *parent = 0);
    static Data *getData();


private:

    static Data *data;
};

#endif // DATAHOLDER_H
