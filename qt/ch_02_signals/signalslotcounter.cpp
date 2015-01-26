#include "signalslotcounter.h"

SignalSlotCounter::SignalSlotCounter(QObject *parent) :
    QObject(parent)
{
}


void SignalSlotCounter::incrementReceived()
{
    emit sendCounterIncrement(++m_counterValue);
    if(m_counterValue == 5)
    {
        emit goodbye();
    }
}
