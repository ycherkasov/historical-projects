#pragma once
#include <QObject>

class MouseFilter : public QObject
{
public:
    MouseFilter(QObject* parent = nullptr);

protected:
    virtual bool eventFilter(QObject * obj, QEvent * ev);
};

