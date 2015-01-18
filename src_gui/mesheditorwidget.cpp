#include "mesheditorwidget.h"
#include "nodefactory.h"

MeshEditorWidget::MeshEditorWidget(QWidget *parent) :
    QWidget(parent)
{
}
void MeshEditorWidget::dragEnterEvent(QDragEnterEvent *event){
    qDebug()<<"dragEnterEvent";
    if (event->mimeData()->hasFormat("application/customthingy")) {
             if (event->source() == this) {
                 event->setDropAction(Qt::MoveAction);
                 event->accept();
             } else {
                 event->acceptProposedAction();
             }
         } else {
             event->ignore();
         }
}

void MeshEditorWidget::dragMoveEvent(QDragMoveEvent *event){
    //qDebug()<<"dragMoveEvent";
}

void MeshEditorWidget::dropEvent(QDropEvent *event){
    qDebug()<<"dropEvent";
}
