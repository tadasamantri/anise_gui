#include "gatebutton.h"
#include <QMouseEvent>

GateButton::GateButton(QString gateName, int nodeID, QWidget *parent) :
    QPushButton(parent)
{
    this->gateName = gateName;
    this->nodeID = nodeID;

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

    //ensure leftclick and no dragging!
    if((e->button() == Qt::LeftButton) && this->rect().contains(e->pos()))
        emit released(this->gateName, this->pos());
}
int GateButton::getNodeID() const
{
    return nodeID;
}

void GateButton::setNodeID(int value)
{
    nodeID = value;
}


