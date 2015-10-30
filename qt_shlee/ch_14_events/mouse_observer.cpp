#include "mouse_observer.h"
#include <QString>
#include <QMouseEvent>

MouseObserver::MouseObserver(QWidget* parent /*= nullptr*/) :
QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    setText("Mouse interactions\n(Press a mouse button)");
}

void MouseObserver::mousePressEvent(QMouseEvent *ev)
{
    dumpEvent(ev, "Mouse pressed");
}

void MouseObserver::mouseReleaseEvent(QMouseEvent *ev)
{
    dumpEvent(ev, "Mouse released");
}

void MouseObserver::mouseMoveEvent(QMouseEvent *ev)
{
    dumpEvent(ev, "Mouse moving");
}

void MouseObserver::mouseDoubleClickEvent(QMouseEvent * ev)
{
    dumpEvent(ev, "Mouse double clicked");
}

void MouseObserver::dumpEvent(QMouseEvent* ev, const QString& message)
{
    QString text = QString("%1\n buttons() = %2\n x() = %3\n y() = %4\n globalX() = %5\n globalY() = %6\n modifiers() = %7").arg(message)
        .arg(buttonsInfo(ev))
        .arg(QString::number(ev->x()))
        .arg(QString::number(ev->y()))
        .arg(QString::number(ev->globalX()))
        .arg(QString::number(ev->globalY()))
        .arg(modifiersInfo(ev));
    setText(text);
}

QString MouseObserver::modifiersInfo(QMouseEvent* ev)
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

QString MouseObserver::buttonsInfo(QMouseEvent* ev)
{
    QString mouse_buttons;

    if (ev->buttons() & Qt::LeftButton){
        mouse_buttons += " Left";
    }
    if (ev->buttons() & Qt::RightButton){
        mouse_buttons += " Right";
    }
    if (ev->buttons() & Qt::MiddleButton){
        mouse_buttons += " Middle";
    }

    return mouse_buttons;
}
