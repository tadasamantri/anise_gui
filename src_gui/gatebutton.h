#ifndef GATEBUTTON_H
#define GATEBUTTON_H

#include <QPushButton>
#include <QString>

class GateButton : public QPushButton {
    Q_OBJECT
public:
    explicit GateButton(QString gateName, QString gateType, int nodeID,
                        bool direction, QWidget *parent = 0);

    QString getGateName() const;
    void setGateName(const QString &value);
    void mouseReleaseEvent(QMouseEvent *e);
    int getNodeID() const;
    void setNodeID(int value);
    void enable();
    void disable();
    bool isInput();
    bool isOutput();
    void setHighlightMode(bool valid);
    void resetPicture();
    QString getGateType() const;
    void setGateType(const QString &value);
    bool getDirection() const;
    void setDirection(bool value);

signals:
    void released(QString gateName, QPoint position);

public slots:

protected:
    bool event(QEvent *e);

private:
    bool direction;
    bool enableClick;
    QString gateName;
    QString gateType;
    int nodeID;
};

#endif  // GATEBUTTON_H
