#pragma once
#include <QWidget>

class QLabel;

class ScreenshotWidget : public QWidget
{
    Q_OBJECT
public:
    ScreenshotWidget(QWidget* parent = nullptr);
    void makeScreenshot();
private:
    QLabel* screen_picture_;
};

