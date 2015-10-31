#include <QPainter>

#include "custom_progress_bar.h"



CustomProgressBar::CustomProgressBar(QWidget* parent /*= nullptr*/) :
QFrame(parent)
{
    // common drawing behavior
    setLineWidth(3);
    setFrameStyle(QFrame::Box | QFrame::Sunken);

    // width and height policy
    // means that provided hint for width is a fixed minimum size
    // hint for height just fixed
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

void CustomProgressBar::setRange(int min_range, int max_range)
{
    min_ = min_range;
    max_ = max_range;
}

void CustomProgressBar::setProgress(int progress)
{
    // emulate setting progress value behavior
    if (progress > max_){
        progress_ = max_;
    }
    if (progress < min_){
        progress_ = min_;
    }
    progress_ = progress;
    
    repaint();

    emit progressChanged(progress_);
}

void CustomProgressBar::paintEvent(QPaintEvent * ev)
{
    // painting stuff
    QPainter p(this);
    QLinearGradient grad(0, 0, width(), height());
    float current_pos = static_cast<float>(progress_) / static_cast<float>(max_ - min_);

    // set linear gradient
    grad.setColorAt(0, Qt::black);
    grad.setColorAt(current_pos, Qt::green);

    //paint control
    p.fillRect(rect(), Qt::black);
    p.setBrush(grad);

    // area of progress
    p.drawRect(0, 0, static_cast<int>(width() * current_pos), height());

    p.setPen(QPen(Qt::green));
    QString text = QString("%1 %").arg(QString::number(progress_));
    p.drawText(rect(), Qt::AlignCenter, text);

    drawFrame(&p);
}

QSize CustomProgressBar::sizeHint() const
{
    return QSize(200, 30);
}
