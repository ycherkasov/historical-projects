#pragma once
#include <QLabel>


class DropWidget : public QLabel
{
    Q_OBJECT
public:
    DropWidget(QWidget* parent = nullptr);
    
protected:

    // event called every time when dragged objects appear in the borders of widget
    virtual void dragEnterEvent(QDragEnterEvent * ev);

    // if the item was dropped in the borders of widget
    virtual void dropEvent(QDropEvent * ev);
};

