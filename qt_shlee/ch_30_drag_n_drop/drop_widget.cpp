#include <QList>
#include <QUrl>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include "drop_widget.h"



DropWidget::DropWidget(QWidget* parent /*= nullptr*/) :
QLabel(parent)
{
    // could process dragEnterEvent/dropEvent
    setAcceptDrops(true);
}

void DropWidget::dragEnterEvent(QDragEnterEvent * ev)
{
    // check whether the widget could accept the item
    if (ev->mimeData()->hasFormat("text/uri-list")){
        ev->acceptProposedAction();
    }
}

void DropWidget::dropEvent(QDropEvent * ev)
{
    QList<QUrl> url_list = ev->mimeData()->urls();
    QString str;
    for (const QUrl& url : url_list){
        str += url.toString();
        str += '\n';
    }
    setText(str);
}
