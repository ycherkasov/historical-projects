#pragma once
#include <QWidget>

class QLabel;

// This class uses QObject timerEvent and process event every 500 msec
class TimerEventWidget : public QWidget
{
    Q_OBJECT
public:
    TimerEventWidget(QWidget* parent = nullptr);
protected:
    virtual void timerEvent(QTimerEvent * ev);
private:
    bool blink_ = false;
    QLabel* text1_;
    QLabel* text2_;
    QLabel* text3_;
};

