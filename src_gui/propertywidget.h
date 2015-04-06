#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include "node.h"

namespace Ui {
class PropertyWidget;
}

class PropertyWidget : public QWidget
{
    friend class MyCheckBox;
    Q_OBJECT

public:
    explicit PropertyWidget(QWidget *parent = 0);
    PropertyWidget(int ID, QWidget *parent = 0);
    ~PropertyWidget();

private:
    Ui::PropertyWidget *ui;
    Node *node;
    int ID;
private slots:
    void nameChanged(QString name);

};
class MyCheckBox : public QCheckBox{
        Q_OBJECT
    public:
        explicit MyCheckBox(QWidget *parent = 0);
        MyCheckBox(QString key, QWidget *parent = 0);

    public slots:
        void toggle();
    private:
        QString key;

    };


#endif // PROPERTYWIDGET_H
