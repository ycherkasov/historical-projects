#include <QTimer>
#include <QString>
#include <QLabel>
#include <QDateTime>

#include "timer_widget.h"


TimerWidget::TimerWidget(QWidget* parent /*= nullptr*/) :
QLabel(parent)
{
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, [this](){
        QString my_time = QDateTime::currentDateTime().toString(Qt::TextDate);
        this->setText(my_time);
    });

    // QTimer::singleShot() set timer work once
    // but we need periodically
    timer->start(500);
}
