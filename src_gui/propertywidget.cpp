#include "propertywidget.h"
#include "ui_propertywidget.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <limits>
char PropertyWidget::count = 0;

PropertyWidget::PropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyWidget)
{
    if(count++ > 0)
        throw("only one window can be opened at once!");
    //count++;
    ui->setupUi(this);
}

PropertyWidget::PropertyWidget(int ID, QWidget *parent) : PropertyWidget(parent)
{
   // ui->setupUi(this);
    this->ID = ID;
    node = Data::instance()->getNodeByID(ID);
    ui->ID->setText(QString::number(this->ID));
    ui->name->setText(node->getName());
    ui->type->setText(node->getType());
    this->setWindowTitle("Node: " + ui->name->text());
    QFormLayout *layout = ui->form;
    QMap<QString, Node::Parameter> *params = node->getParams();
    QStringList keys = params->keys();
    for (QString key : keys) {
        Node::Parameter p = (*params)[key];
        QWidget *item = 0;
        QVariant::Type type = QVariant::nameToType(p.type.toUtf8());
        if (type == QVariant::Bool) {
            MyCheckBox *checkbox = new MyCheckBox(key, this);
            checkbox->setChecked(p.value.toBool());
            item = checkbox;
        }

        else if (type == QVariant::String) {
            QLineEdit *textLine = new TextLine(this->ID, key, this);
            textLine->setText(p.value.toString());
            item = textLine;
            if (key.contains("file", Qt::CaseInsensitive)) {
                QWidget *tmp = new QWidget(this);
                FileButton *button = new FileButton(key, ID, textLine, tmp);
                button->setText("File...");
                QHBoxLayout *tmp_layout = new QHBoxLayout(tmp);
                tmp_layout->addWidget(textLine);
                tmp_layout->addWidget(button);
                qDebug() << "prop_window file selcetor layout: " << tmp_layout->widget();
                item = tmp;
            }
        }

        else if (type == QVariant::Int || type == QVariant::UInt || type == QVariant::Double) {
            QAbstractSpinBox *spinBox = 0;
            if(type == QVariant::Double){
                spinBox = new DoubleSpinBox(this->ID,key,this);
                static_cast<QDoubleSpinBox*>(spinBox)->setMaximum(std::numeric_limits<double>::max());
                static_cast<QDoubleSpinBox*>(spinBox)->setValue(p.value.toDouble());
            }
            else {
                spinBox = new SpinBox(this->ID,key,this);
                static_cast<QSpinBox*>(spinBox)->setMaximum(std::numeric_limits<int>::max());
                static_cast<QSpinBox*>(spinBox)->setValue(p.value.toInt());
                if (type == QVariant::UInt)
                    static_cast<QSpinBox*>(spinBox)->setMinimum(0);
            }
            item = spinBox;            
        }
        if(item){
            QLabel *name = new QLabel(p.name,this);
            name->setToolTip(p.descr);
            item->setToolTip(p.descr);
            layout->addRow(name, item);
        }
    }
}

PropertyWidget::~PropertyWidget()
{
    count--;
    delete ui;
}

bool PropertyWidget::close(){
    bool result = QWidget::close();
    delete this;
    return result;
}

void PropertyWidget::closeEvent(QCloseEvent *e){
    QWidget::closeEvent(e);
    this->deleteLater();
    e->accept();
}

void PropertyWidget::nameChanged(QString name)
{
    if(node->getName() == name)
        return;
    node->setName(name);
    this->setWindowTitle("Node: " + name);
}
