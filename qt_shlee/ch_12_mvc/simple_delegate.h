#pragma once
#include <QItemDelegate>

// Delegate is responsible for the single View element draw and edit, also read from Model and write back to model

class SimpleDelegate : public QItemDelegate
{
public:
    SimpleDelegate(QObject* parent);

    // Here we define only delegate drawings
    virtual void paint(QPainter* painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

    // if we want to redefine edit, we should implement 
    // createEditor() - create Widget in place of element
    // setEditorData() - load data from model
    // setModelData() - write data to model
    // sizeHint() - size of the delegate editor
    // updateEditorGeometry() update editor placement
};

