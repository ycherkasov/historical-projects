#pragma once

#include <QObject>


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