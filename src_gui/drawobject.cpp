#include "drawobject.h"
#include <QPixmap>
#include <QWidget>


DrawObject::DrawObject(QWidget *parent = 0)
{
    this->widget = new QWidget(parent);
    this->labelOfThis = new QLabel(this->widget);
}
