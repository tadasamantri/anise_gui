#include "parameterwindow.h"
#include "data.h"
#include <QPixmap>
#include <QShortcut>
#include <QDebug>
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include <QTextLine>
#include <QAbstractButton>
#include <QSpinBox>

/**
 * @brief ParameterWindow::ParameterWindow
 * @param parent
 * @param nodeID
 */
ParameterWindow::ParameterWindow(QWidget *parent, int nodeID)
    : QWidget(parent) {
    // A object for working on the layout
    layout = new QFormLayout(this);
    this->setLayout(layout);

    this->nodeID = nodeID;

    // save the nodeID in node
    node = Data::instance()->getMesh()->getNodeByID(nodeID);

    // to work with the params of the node
    params = node->getParams();

    // Todo: Have to implement a run method in the MainWindow
    // start the popup...
    init();

    this->show();
}

/**
 * @brief ParameterWindow::init
 * Creating the popupwindow and add the informations
 * Checkbox
 * Sinbox
 * Textlines
 */
void ParameterWindow::init() {
    foreach (QString key, params->keys()) {
        QVariant value = params->find(key).value();

        switch (value.userType()) {
        case QVariant::Bool:
            addCheckbox(key);
            break;
        case QVariant::Int:
            addSpinbox(key);
            break;
        case QVariant::String:
            addTextline(key);
            break;

        default:
            break;
        }
    }
}

/**
 * @brief ParameterWindow::addCheckbox
 * Adds a checkbox to the Popup Layout.
 * @param key
 */
void ParameterWindow::addCheckbox(const QString &key) {
    QCheckBox *check = new QCheckBox(key);
    layout->addRow(key, check);
    check->setCheckState((params->find(key).value().toBool()) ? Qt::Checked
                                                              : Qt::Unchecked);
}

/**
 * @brief ParameterWindow::addSpinbox
 * Adds a Spinbox to the Popup Layout
 * @param key
 */
void ParameterWindow::addSpinbox(const QString &key) {
    QSpinBox *spinner = new QSpinBox();
    layout->addRow(key, spinner);
}
/**
 * @brief ParameterWindow::addTextline
 * Adds a Textline to the Popup Layout
 * @param key
 */
void ParameterWindow::addTextline(const QString &key) {
    QTextEdit *text = new QTextEdit();
    layout->addRow(key, text);
    text->setText(params->find(key).value().toString());
}
