#ifndef PARAMETERWINDOW_H
#define PARAMETERWINDOW_H

#include <QWidget>
#include "node.h"
#include <QFormLayout>

class ParameterWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterWindow(QWidget *parent = 0);

    ParameterWindow(QWidget *parent, int nodeID);
    void init();



signals:



public slots:


private:

    int nodeID;
    QFormLayout *layout;
    Node* node;

    QVariantMap* params;
void addCheckbox(const QString &key);

void addSpinbox(const QString &key);
void addTextline(const QString &key);
};

#endif // PARAMETERWINDOW_H
