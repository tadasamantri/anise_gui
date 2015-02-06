#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class DragWidget : public QWidget {
    Q_OBJECT
public:
    explicit DragWidget(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif  // DRAGWIDGET_H
