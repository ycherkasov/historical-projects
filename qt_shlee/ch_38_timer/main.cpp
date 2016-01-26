#include <QApplication>
#include <QTabWidget>

#include "date_time_widget.h"
#include "timer_event_widget.h"
#include "timer_widget.h"

// * QDate could be created from string, numeric, currentDate() method
// * the same about time
// * QDateTime encapsulates QDate and QTime
// * date/time format could be text or predefined (DateFormat enum)
// * Timer could be implemented in two ways:
// - QObject::startTimer() method and process timerEvent()
// - create QTimer or QBasicTimer object and connect timeout() signal to slot
// * QTimer::singleShot() set timer work once

template <typename T>
T* get_widget(QWidget* parent){
    return new T(parent);
}


int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    
    QTabWidget wnd;
    wnd.addTab(get_widget<DateTimeWidget>(&wnd), "Date Time");
    wnd.addTab(get_widget<TimerEventWidget>(&wnd), "Timer event");
    wnd.addTab(get_widget<TimerWidget>(&wnd), "Timer object");

    wnd.resize(300, 200);

    wnd.show();

    return app.exec();
}
