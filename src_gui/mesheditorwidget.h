#ifndef MESHEDITORWIDGET_H
#define MESHEDITORWIDGET_H

#include <QWidget>

#include <QWidget>
#include <QEvent>

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QDrag>
#include <QDebug>
#include <Qt>
#include <QMimeData>
#include <QPainter>
#include <QObject>

#include "drawobject.h"

class MeshEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit MeshEditorWidget(QWidget *parent = 0);
    QPoint dragStartPosition;

    //returns true if there is a child of Meshfield containing drawObject with objectID
    //function to confirm drawObject got deleted
    //
    bool containsID(int objectID);

protected:
    void mousePressEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void paintEvent(QPaintEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);


private:


signals:

    void onWidgetClicked(int nodeID);

};

#endif  // MESHEDITORWIDGET_H
