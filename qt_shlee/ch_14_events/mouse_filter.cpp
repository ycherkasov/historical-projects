#include "mouse_filter.h"
#include <QMouseEvent>
#include <QMessageBox>


MouseFilter::MouseFilter(QObject* parent /*= nullptr*/) :
QObject(parent)
{
}

bool MouseFilter::eventFilter(QObject * obj, QEvent * ev)
{
    if (ev->type() == QEvent::MouseButtonPress){
        if (QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(ev)){
            if (mouse_event->button() == Qt::LeftButton){

                QString receiver_name = obj->metaObject()->className(); 
                QMessageBox::information(0, "Class Name", receiver_name);

                // true means block passing event to receiver
                return true;
            }
        }
    }
    return false;
}
