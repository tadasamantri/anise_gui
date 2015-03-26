#ifndef MESHEDITORWIDGET_H
#define MESHEDITORWIDGET_H

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
#include <QVector>

#include "drawobject.h"
#include "nodefactory.h"

#include "node.h"

struct NewLine{

    bool drawLine;
    int srcNodeID;
    QString srcGateName;
    int destNodeID;
    QString destGateName;
    QVector<QPoint> wayPoints;


};




class MeshEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit MeshEditorWidget(QWidget *parent = 0);
    QPoint dragStartPosition;

    //returns true if there is a child of Meshfield containing drawObject with objectID
    //function to confirm drawObject got deleted
    //
    bool containsID(int objectID);

    void connectSignals();
protected:

    //variables for drawing a line to the mouse cursor
    bool drawLine;
    QPoint mousePosition;
    QPoint mousePositionOld;
    int mouseMoveDistance;

    QVector<QPoint> lineWayPoints;
    void mousePressEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);



private:

    NewLine newLine;

    //Clears new Line which is/was in construction
    void clearNewLine();
    void resetToEditMode();

    bool correctGate(int nodeID, QString gateName);

signals:

    void onWidgetClicked(int nodeID);
    void drawLineModeChanged();

public slots:

    bool handleGateClick(int nodeID, QString gateName, QPoint position);
private slots:
    void changeLineDrawMode();
};

#endif  // MESHEDITORWIDGET_H
