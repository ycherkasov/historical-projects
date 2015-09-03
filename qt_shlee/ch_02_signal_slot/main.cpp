#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include "counter.h"

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
    QObject::connect(&counter, SIGNAL(exit_me()), &app, SLOT(quit()));

    // processing loop here
    return app.exec();
}