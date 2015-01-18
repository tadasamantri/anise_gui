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

class MeshEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MeshEditorWidget(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // MESHEDITORWIDGET_H
