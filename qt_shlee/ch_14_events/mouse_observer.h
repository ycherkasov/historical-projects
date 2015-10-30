#pragma once
#include <QLabel>

class MouseObserver : public QLabel
{
public:
    MouseObserver(QWidget* parent = nullptr);
protected:
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void mouseDoubleClickEvent(QMouseEvent * ev);

    void dumpEvent(QMouseEvent* ev, const QString& message);
    QString modifiersInfo(QMouseEvent* ev);
    QString buttonsInfo(QMouseEvent* ev);
};

