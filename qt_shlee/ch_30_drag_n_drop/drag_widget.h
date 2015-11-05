#pragma once
#include <QLabel>
#include <QPoint>

class DragWidget : public QLabel
{
    Q_OBJECT
public:
    DragWidget(QWidget* parent = nullptr);

    // called when the drag event is detected
    void startDrag();

protected:

    // drag event processing
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    
private:
    QPoint start_drag_position_;
};

