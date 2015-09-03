#pragma once

#include <QObject>

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

class Counter : public QObject
{
    Q_OBJECT

public:
    Counter();

public slots:

    // processing signals of incrementing value
    void increment_slot();

signals:
    // signals do not have implementation in the class
    // they processed by MOC in a separate *.cpp
    
    // custom signal for exiting application
    void exit_me();

    // custom signal with parameter
    void counter_changed(int);

private:
    int value_;
};