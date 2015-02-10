#include "gatebutton.h"

GateButton::GateButton(QString gateName, QWidget *parent) :
    QPushButton(parent)
{
    this->gateName = gateName;
}

QString GateButton::getGateName() const
{
    return gateName;
}

void GateButton::setGateName(const QString &value)
{
    gateName = value;
}

void GateButton::mouseReleaseEvent(QMouseEvent *e)
{
    QPushButton::mouseReleaseEvent(e);
    emit released(this->gateName, this->pos());
}

