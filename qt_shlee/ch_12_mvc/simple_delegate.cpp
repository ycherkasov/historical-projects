#include <QPainter>

#include "simple_delegate.h"


SimpleDelegate::SimpleDelegate(QObject* parent)
: QItemDelegate(parent)
{
}

void SimpleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // we just redefine painting of the single element
    if (option.state & QStyle::State_MouseOver)
    {
        // drawing area
        QRect rect = option.rect;

        // draw background
        QLinearGradient grad(0, 0, rect.width(), rect.height());
        grad.setColorAt(0, Qt::white);
        grad.setColorAt(0.5, Qt::blue);
        grad.setColorAt(1.0, Qt::green);

        painter->setBrush(grad);
        painter->drawRect(rect);
    }
    QItemDelegate::paint(painter, option, index);
}
