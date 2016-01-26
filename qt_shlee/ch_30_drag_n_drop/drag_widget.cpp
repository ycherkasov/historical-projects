#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

#include "drag_widget.h"



DragWidget::DragWidget(QWidget* parent /*= nullptr*/) :
QLabel(parent)
{
    setText("This text is able to be drag-n-dropped");
}



void DragWidget::startDrag()
{
    // Store widget text in mime data
    QMimeData* mime_data = new QMimeData;
    mime_data->setText(text());

    // Create drag event
    QDrag* drg = new QDrag(this);
    drg->setMimeData(mime_data);
    drg->exec();
}

void DragWidget::mousePressEvent(QMouseEvent *ev)
{
    // here we should detect that dragging is started

    // left button is pressed, dragging may start
    if (ev->button() == Qt::LeftButton){
        start_drag_position_ = ev->pos();
    }

    // process parent as usual
    QLabel::mousePressEvent(ev);
}

void DragWidget::mouseMoveEvent(QMouseEvent *ev)
{
    // we still hold left button while moving
    if (ev->buttons() & Qt::LeftButton){

        // distance we moved
        // the sum of the absolute values of x() and y(), 
        // traditionally known as the "Manhattan length" of the vector from the origin to the point
        QPoint pt = ev->pos() - start_drag_position_;
        int distance = pt.manhattanLength();

        // ensure that the dragging is not performed accidentally
        if (distance > QApplication::startDragDistance()){
            startDrag();
        }

    }
    // process parent as usual
    QLabel::mouseMoveEvent(ev);

}
