#pragma once
#include <QTextEdit>

// First tried QLabel, but QLable does not process QKeyEvent
class KeyboardObserver : public QTextEdit
{
public:
    KeyboardObserver(QWidget* parent = nullptr);
protected:
    virtual void keyPressEvent(QKeyEvent *ev);
    QString modifiersInfo(QKeyEvent* ev);
};

