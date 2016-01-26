#pragma once
#include <QObject>

// Filter mouse button press events
class MouseFilter : public QObject
{
public:
    MouseFilter(QObject* parent = nullptr);

protected:
    // we can do anything with 'obj', even delete
    virtual bool eventFilter(QObject * obj, QEvent * ev);
};

