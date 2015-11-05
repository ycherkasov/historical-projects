#include "screenshot_widget.h"
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QLayout>

ScreenshotWidget::ScreenshotWidget(QWidget* parent /*= nullptr*/)
{
    // scaled screen size
    resize(640, 480);

    screen_picture_ = new QLabel;

    QPushButton* make_screenshot = new QPushButton("Make screenshot");

    connect(make_screenshot, &QPushButton::clicked, this, &ScreenshotWidget::makeScreenshot);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(make_screenshot);
    layout->addWidget(screen_picture_);
    setLayout(layout);
}

void ScreenshotWidget::makeScreenshot()
{
    QDesktopWidget* pwgt = QApplication::desktop();
    QPixmap pic = QPixmap::grabWindow(pwgt->screen()->winId());
    screen_picture_->setPixmap(pic.scaled(screen_picture_->size()));
}
