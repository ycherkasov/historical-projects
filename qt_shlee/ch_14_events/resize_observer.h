#pragma once
#include <QLabel>


class ResizeObserver : public QLabel
{
public:
    ResizeObserver(QWidget* parent = nullptr);
    virtual void resizeEvent(QResizeEvent * ev);
};

