#ifndef GATEBUTTON_H
#define GATEBUTTON_H

#include <QPushButton>
#include <QString>

class GateButton : public QPushButton
{
    Q_OBJECT
public:
    explicit GateButton(QString gateName, QWidget *parent = 0);

    QString getGateName() const;
    void setGateName(const QString &value);
    void mouseReleaseEvent(QMouseEvent *e);

signals:

    void released(QString gateName, QPoint position);

public slots:

private:

    QString gateName;

};

#endif // GATEBUTTON_H
