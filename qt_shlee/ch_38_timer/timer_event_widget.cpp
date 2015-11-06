#include <QLabel>
#include <QLayout>

#include "timer_event_widget.h"


TimerEventWidget::TimerEventWidget(QWidget* parent /*= nullptr*/) :
QWidget(parent),
text1_(new QLabel),
text2_(new QLabel),
text3_(new QLabel)
{
    // in ms
    startTimer(500);

    // set Layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(text1_);
    layout->addWidget(text2_);
    layout->addWidget(text3_);
    setLayout(layout);
}

void TimerEventWidget::timerEvent(QTimerEvent * ev)
{
    blink_ = !blink_;
    if (blink_){
        text1_->setText("<FONT COLOR = RED><CENTER>Blink</CENTER></FONT>");
        text2_->setText("<FONT COLOR = GREEN><CENTER>Blink</CENTER></FONT>");
        text3_->setText("<FONT COLOR = BLUE><CENTER>Blink</CENTER></FONT>");
    }
    else{
        text1_->setText("<FONT COLOR = BLUE><CENTER>Blink</CENTER></FONT>");
        text2_->setText("<FONT COLOR = RED><CENTER>Blink</CENTER></FONT>");
        text3_->setText("<FONT COLOR = GREEN><CENTER>Blink</CENTER></FONT>");
    }
}
