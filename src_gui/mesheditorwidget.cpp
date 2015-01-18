#include "mesheditorwidget.h"
#include "nodefactory.h"
#include "dataholder.h"
#include "data.h"
#include "node.h"

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

    Node *newNode = NodeFactory::createTestNode();
    //newNode.setPosition();

    qDebug() << "x:" << newNode->position_x << " y: " << newNode->position_y;
    DataHolder::getData()->addNodeToMesh(newNode);
}
