#include "dataholder.h"

Data *DataHolder::data = 0;

DataHolder::DataHolder(QObject *parent) : QObject(parent) {}

Data *DataHolder::getData() {

  if (data == NULL)
    data = new Data();

  return data;
}
