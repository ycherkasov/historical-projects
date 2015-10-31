#pragma once
#include <QFrame>


// Our progress bar is based on QFrame, so we draw completely on our own
class CustomProgressBar : public QFrame
{
    Q_OBJECT
public:
    CustomProgressBar(QWidget* parent = nullptr);

    // emulate standard progress
    void setRange(int min_range, int max_range);

    void setProgress(int progress);

    int getProgress() const { 
        return progress_; 
    }

    int getMin() const {
        return min_;
    }

    int getMax() const {
        return max_;
    }

    virtual QSize sizeHint() const;

signals:

    void progressChanged(int progress);

protected:

    // our our draw procedure
    virtual void paintEvent(QPaintEvent * ev);

private:
    int min_ = 0;
    int max_ = 0;
    int progress_ = 0;
};

