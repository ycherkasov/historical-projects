#pragma once
#include <QLabel>

// Output mouse information
class MouseObserver : public QLabel
{
public:
    MouseObserver(QWidget* parent = nullptr);
protected:
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void mouseDoubleClickEvent(QMouseEvent * ev);

    // set event text
    void dumpEvent(QMouseEvent* ev, const QString& message);

    // whether Alt/Ctrl/Shift pressed
    QString modifiersInfo(QMouseEvent* ev);

    // whether mouse buttons pressed
    QString buttonsInfo(QMouseEvent* ev);
};

