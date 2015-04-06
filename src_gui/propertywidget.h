#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H
#include <QFileDialog>
#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "node.h"
#include "data.h"

namespace Ui {
class PropertyWidget;
}

class PropertyWidget : public QWidget
{
    friend class MyCheckBox;
    friend class TextLine;
    friend class FileButton;
    Q_OBJECT

public:
    explicit PropertyWidget(QWidget *parent = 0);
    PropertyWidget(int ID, QWidget *parent = 0);
    ~PropertyWidget();
public slots:
    bool close();
private:
    Ui::PropertyWidget *ui;
    static char count;
    Node *node;
    int ID;
    void closeEvent(QCloseEvent *e);
private slots:
    void nameChanged(QString name);
};
class MyCheckBox : public QCheckBox{
        Q_OBJECT
    public:
        explicit MyCheckBox(QWidget *parent = 0);
    MyCheckBox(QString key, QWidget *parent = 0) : QCheckBox(parent)
    {
        this->key = key;
    }

    public slots:
        //void update();
    private:
        QString key;

        void mouseReleaseEvent(QMouseEvent *e)
        {
            QCheckBox::mouseReleaseEvent(e);
            PropertyWidget *p = dynamic_cast<PropertyWidget*>(parent());
            Data::instance()->getNodeByID(p->ID)->setParam(key,this->checkState() == Qt::Checked);
            Data::instance()->getMainWindow()->updatePropertyTable(p->ID);
        }
};

class FileButton: public QPushButton{
    Q_OBJECT
public:
    explicit FileButton(QWidget *parent = 0);
    FileButton(QString key, int ID, QLineEdit *text, QWidget *parent = 0) : QPushButton(parent){
        this->key = key;
        this->ID = ID;
        textLine = text;
    }

private:
    QString key;
    int ID;
    QLineEdit *textLine;
    void mouseReleaseEvent(QMouseEvent *e){
        QPushButton::mouseReleaseEvent(e);
        QString text;
        if(key.contains("out") || Data::instance()->getNodeByID(ID)->getParamStructByKey(key).name.contains("out", Qt::CaseInsensitive)){
            text = QFileDialog::getSaveFileName(0,"Select File...",QFileInfo(textLine->text()).dir().absolutePath());
        }
        else
            text = QFileDialog::getOpenFileName(0,"Select File...",QFileInfo(textLine->text()).dir().absolutePath());
        if(text == "")
            return;
        textLine->setText(text);
        int ID = dynamic_cast<PropertyWidget*>(parent()->parent())->ID;
        Data::instance()->getNodeByID(ID)->setParam(key,text);
        Data::instance()->getMainWindow()->updatePropertyTable(ID);
    }

};

class TextLine: public QLineEdit{
    friend class PropertyWidget;
    Q_OBJECT
public:
    explicit TextLine(QWidget *parent = 0);
    TextLine(int ID, QString key, QWidget *parent = 0) : QLineEdit(parent){
        this->key = key;
        this->ID = ID;
    }
private:
    QString key;
    int ID;
    void keyPressEvent(QKeyEvent *e){
        QLineEdit::keyPressEvent(e);
        if(text() != Data::instance()->getNodeByID(ID)->getParamByKey(key).toString()){
            Data::instance()->getNodeByID(ID)->setParam(key,this->text());
            Data::instance()->getMainWindow()->updatePropertyTable(ID);
        }
    }
};

class SpinBox: public QSpinBox{
    Q_OBJECT
public:
    explicit SpinBox(QWidget *parent = 0);
    SpinBox(int ID, QString key, QWidget *parent) : QSpinBox(parent){
        this->ID = ID;
        this->key = key;
        connect(this,SIGNAL(valueChanged(int)),this,SLOT(changeValue(int)));
    }
private:
    int ID;
    QString key;
private slots:
    void changeValue(int value){
        Data::instance()->getNodeByID(ID)->setParam(key,value);
        Data::instance()->getMainWindow()->updatePropertyTable(ID);
    }
};

class DoubleSpinBox: public QDoubleSpinBox{
    Q_OBJECT
public:
    explicit DoubleSpinBox(QWidget *parent = 0);
    DoubleSpinBox(int ID, QString key, QWidget *parent) : QDoubleSpinBox(parent){
        this->ID = ID;
        this->key = key;
        connect(this,SIGNAL(valueChanged(double)),this,SLOT(changeValue(double)));
    }
private:
    int ID;
    QString key;
private slots:
    void changeValue(double value){
        Data::instance()->getNodeByID(ID)->setParam(key,value);
        Data::instance()->getMainWindow()->updatePropertyTable(ID);
    }
};

#endif // PROPERTYWIDGET_H
