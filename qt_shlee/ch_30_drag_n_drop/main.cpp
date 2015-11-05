#include <QApplication>
#include <QTabWidget>

#include "drag_widget.h"
#include "drop_widget.h"

// TODO: example with internal D'n'D

// * QClipboard class statically accessed from QApplication::clipboard()
// * clipboard is a static area of memory that could be accessed by any app
// * QMimeData stores information about dragged item types (setText, setHtml, setImage, setData)
// It could be text, image, of just QBytesArray with set text MIME-type (setData method)
// * dragLeaveEvent - dragged item leaves widget
// * dragEnterEvent - dragged item enters widget
// * QDrag - object for the dragging process (setMimeData, setPixmap - icon while dragging etc.)
// * for catching startDrag event process mousePressEvent+mouseMoveEvent
// * QApplication::startDragDistance() prevents from accidental drag-n-drop
// * if dragged item does mot leave application, MIME data is not obligatory
// Just pass a pointer to data (see Shlee 30)

template <typename T>
T* get_widget(QWidget* parent){
    return new T(parent);
}


int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    
    QTabWidget wnd;
    wnd.addTab(get_widget<DragWidget>(&wnd), "Drag");
    wnd.addTab(get_widget<DropWidget>(&wnd), "Drop");

    wnd.resize(300, 200);

    wnd.show();

    return app.exec();
}
