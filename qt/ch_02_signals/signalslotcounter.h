#ifndef SIGNALSLOTCOUNTER_H
#define SIGNALSLOTCOUNTER_H

#include <QObject>

// The class represents custom signal and slots mechanism
// Could be both signal and slot
class SignalSlotCounter : public QObject
{
    Q_OBJECT
public:
    explicit SignalSlotCounter(QObject *parent = 0);

// signals could not be private-protected-public
signals:

    void sendCounterIncrement(size_t val);

    void goodbye();

// slots could be private-protected-public
// slots could be virtual but it is really slow
public slots:

    void incrementReceived();

private:

    size_t m_counterValue = 0;
};

#endif // SIGNALSLOTCOUNTER_H
