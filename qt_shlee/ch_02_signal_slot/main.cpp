#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include "counter.h"

// * any Q_OBJECT class could have any number of signals and slots
// * any signal could have any arguments
// * signal could be connected to several slots
// * slot could accept any number of different signals
// * signal-slot could be connected-disconnected in any point of application
// * when object is destroyed all signals are disconnected
// * signal processing is a bit slower than callback
// * signal could not be public/protected/private
// * slot could be public/protected/private
// * connection could be thread-safe. It is detected automatically
// * blockSignals() lock sending signals by the object
// * multiple inheritance from QObject is not allowed!
// * slot could determine the emitter using sender() method

// Metainformation that contained by QObject:
// * names
// * signal-slots
// * timers (QTimer)
// * actions (QAction)
// * casting info (qobject_cast)
// * properties

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    QLabel lbl("0");
    QPushButton btn("Add");
    Counter counter;

    lbl.show();
    btn.show();

    // on click button send a signal to counter
    QObject::connect(&btn, SIGNAL(clicked()), &counter, SLOT(increment_slot()));

    // on a custom signal from counter increment label value (standard Qt slot)
    // Parameter types should be the same!
    QObject::connect(&counter, SIGNAL(counter_changed(int)), &lbl, SLOT(setNum(int)));

    // on a custom signal from counter exit application (standard Qt slot)
    // Use new syntax better! It is compile-time checked!
    QObject::connect(&counter, &Counter::exit_me, &app, &QApplication::quit);

    // processing loop here
    return app.exec();
}