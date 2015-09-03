#include "counter.h"


Counter::Counter() : value_(){}

void Counter::increment_slot()
{
    // we don't care whether signal will be processed
    emit counter_changed(++value_);

    if (value_ == 5){
        emit exit_me();
    }
}