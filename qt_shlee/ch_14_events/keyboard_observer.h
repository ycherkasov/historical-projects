#pragma once
#include <QTextEdit>

// First tried QLabel, but QLable does not process QKeyEvent
class KeyboardObserver : public QTextEdit
{
public:
    KeyboardObserver(QWidget* parent = nullptr);
protected:

    // redefine event processing
    virtual void keyPressEvent(QKeyEvent *ev);

    // whether Alt/Ctrl/Shift pressed
    QString modifiersInfo(QKeyEvent* ev);
};

