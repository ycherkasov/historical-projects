#include "resize_observer.h"
#include <QString>
#include <QResizeEvent>


ResizeObserver::ResizeObserver(QWidget* parent /*= nullptr*/) :
QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
}

void ResizeObserver::resizeEvent(QResizeEvent * ev)
{
    QString text = QString("Width = %1\nHeight = %2")
        .arg(QString::number(ev->size().width()))
        .arg(QString::number(ev->size().height()));
    setText(text);
}
