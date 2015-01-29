#ifndef NODETYPELABEL_H
#define NODETYPELABEL_H

#include <QLabel>

class NodeTypeLabel : public QLabel
{
    Q_OBJECT
public:
    explicit NodeTypeLabel(QWidget *parent = 0);

    QString getType() const;
    void setType(const QString &value);

signals:

public slots:

private:
    QString type;

};

#endif // NODETYPELABEL_H
