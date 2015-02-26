#include "gatebutton.h"
#include <QMouseEvent>
#include "data.h"

GateButton::GateButton(QString gateName, QString gateType, int nodeID, bool direction, QWidget *parent)
    : QPushButton(parent) {

    this->gateType = gateType;
    this->gateName = gateName;
    this->nodeID = nodeID;
    this->enableClick = true;
    this->direction= direction;

    this->setToolTip(gateType);


}

QString GateButton::getGateName() const { return gateName; }

void GateButton::setGateName(const QString &value) { gateName = value; }

void GateButton::mouseReleaseEvent(QMouseEvent *e) {
    QPushButton::mouseReleaseEvent(e);

    // ensure leftclick and no dragging!
    if ((e->button() == Qt::LeftButton) && this->rect().contains(e->pos())){

        emit released(this->gateName, this->pos());

    }
}
int GateButton::getNodeID() const { return nodeID; }

void GateButton::setNodeID(int value) { nodeID = value; }

void GateButton::enable() { enableClick = true; }

void GateButton::disable() { enableClick = false; }

bool GateButton::event(QEvent *e) {
    if (enableClick)
        QPushButton::event(e);

    else {
        if (e->type() == QEvent::Paint || e->type() == QEvent::ToolTip) {
            return QPushButton::event(e);
        }
    }
    return true;
}
QString GateButton::getGateType() const
{
    return gateType;
}

void GateButton::setGateType(const QString &value)
{
    gateType = value;
}



void GateButton::setHighlightMode(bool valid){

    if(valid)
        this->setIcon(*SingletonRender::instance()->getImage("gate-green.png"));
    else
        this->setIcon(*SingletonRender::instance()->getImage("gate-red.png"));

}

void GateButton::resetPicture(){

    this->setIcon(*SingletonRender::instance()->getImage("gate.png"));

}


bool GateButton::isInput()
{
 return direction;
}

bool GateButton::isOutput(){

    return !direction;
}
