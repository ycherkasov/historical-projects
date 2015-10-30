#include "keyboard_observer.h"
#include <QMouseEvent>
#include <QString>

KeyboardObserver::KeyboardObserver(QWidget* parent /*= nullptr*/):
QTextEdit(parent)
{
    setText("Make the widget focused");
}

void KeyboardObserver::keyPressEvent(QKeyEvent *ev)
{
    QString text = QString("Key pressed, code = %1, modifier = %2").arg(ev->key()).arg(modifiersInfo(ev));
    setText(text);
}


QString KeyboardObserver::modifiersInfo(QKeyEvent* ev)
{
    QString moditiers_str;

    if (ev->modifiers() & Qt::ShiftModifier) {
        moditiers_str += "Shift ";
    }
    if (ev->modifiers() & Qt::ControlModifier) {
        moditiers_str += "Control ";
    }
    if (ev->modifiers() & Qt::AltModifier) {
        moditiers_str += "Alt";
    }
    return moditiers_str;
}
