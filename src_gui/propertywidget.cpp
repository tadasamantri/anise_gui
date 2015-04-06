#include "propertywidget.h"
#include "ui_propertywidget.h"
#include "data.h"
#include <QFormLayout>

PropertyWidget::PropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyWidget)
{
    ui->setupUi(this);
}

PropertyWidget::PropertyWidget(int ID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyWidget)
{
    ui->setupUi(this);
    this->ID = ID;
    node = Data::instance()->getNodeByID(ID);
    ui->ID->setText(QString::number(this->ID));
    ui->name->setText(node->getName());
    ui->type->setText(node->getType());
    this->setWindowTitle("Node: " + ui->name->text());
    QFormLayout *layout = ui->form;
    QMap<QString, Node::Parameter> *params = node->getParams();
    QStringList keys = params->keys();
    for(QString key : keys){
        Node::Parameter p = (*params)[key];
        QWidget *item = 0;
        switch (p.value.type()) {
        case QVariant::Bool:
            item = (QWidget*) new MyCheckBox(key,this);
            break;
        case QVariant::String:
            item = new QLineEdit(p.value.toString(),this);
            break;
        case QVariant::Int:
            //item = new QSpinB
        default:
            ;
            break;
        }
        layout->addRow(p.name,item);
    }
}

PropertyWidget::~PropertyWidget()
{
    delete ui;
}

void PropertyWidget::nameChanged(QString name)
{
    if(node->getName() == name)
        return;
    node->setName(name);
    this->setWindowTitle("Node: " + name);
}


MyCheckBox::MyCheckBox(QString key, QWidget *parent) : QCheckBox(parent)
{
    this->key = key;
}

void MyCheckBox::toggle()
{
    QCheckBox::toggle();
    PropertyWidget *p = dynamic_cast<PropertyWidget*>(parent());
    Data::instance()->getNodeByID(p->ID)->setParam(key,this->checkState() == Qt::Checked);
}
