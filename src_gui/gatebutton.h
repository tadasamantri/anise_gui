#ifndef GATEBUTTON_H
#define GATEBUTTON_H

#include <QPushButton>
#include <QString>

class GateButton : public QPushButton
{
    Q_OBJECT
public:
    explicit GateButton(QString gateName, int nodeID, QWidget *parent = 0);

    QString getGateName() const;
    void setGateName(const QString &value);
    void mouseReleaseEvent(QMouseEvent *e);

    int getNodeID() const;
    void setNodeID(int value);

    void enable();
    void disable();

    void changeMask();
signals:

    void released(QString gateName, QPoint position);

public slots:

protected:
    bool event(QEvent *e);
private:


    bool enableClick;
    QString gateName;
    int nodeID;

};

#endif // GATEBUTTON_H
