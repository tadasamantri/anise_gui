#include "nodetypelabel.h"

NodeTypeLabel::NodeTypeLabel(QWidget *parent) :
    QLabel(parent)
{
}
QString NodeTypeLabel::getType() const
{
    return type;
}

void NodeTypeLabel::setType(const QString &value)
{
    type = value;
}

