#pragma once
#include <QLabel>

// Output resize information
class ResizeObserver : public QLabel
{
public:
    ResizeObserver(QWidget* parent = nullptr);

protected:
    // catch window size changed
    virtual void resizeEvent(QResizeEvent * ev);
};

