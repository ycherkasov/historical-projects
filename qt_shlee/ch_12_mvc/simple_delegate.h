#pragma once
#include <QItemDelegate>

class SimpleDelegate : public QItemDelegate
{
public:
    SimpleDelegate(QObject* parent);

    virtual void paint(QPainter* painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
};

