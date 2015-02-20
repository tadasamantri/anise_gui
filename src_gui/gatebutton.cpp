#include "gatebutton.h"
#include <QMouseEvent>
#include "data.h"

GateButton::GateButton(QString gateName, int nodeID, QWidget *parent) :
    QPushButton(parent)
{
    this->gateName = gateName;
    this->nodeID = nodeID;
    this->enableClick = true;

    Node *node = Data::instance()->getMesh()->getNodeByID(nodeID);
    //has to ask this, cause gatebutton is used in Nodecatalog also
    //in this case node isn't in mesh respectively = 0
    if(node)
        this->setToolTip(node->getGateByName(gateName)->getType());

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

void GateButton::enable()
{
    enableClick = true;
}

void GateButton::disable()
{
    enableClick = false;
}

bool GateButton::event ( QEvent * e ) {

    if(enableClick)
        QPushButton::event(e);

    else{


    if (e->type() == QEvent::Paint ||
            e->type() == QEvent::ToolTip) {
      return QPushButton::event(e);
   }
    }
   return true;
}

